// Este es el main.cpp que junto a menu.cpp menu.h camera_params.h crea un menu bonito que hace config.
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "utils.h"
#include "menu.h"
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

using namespace std;
using namespace cv;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

bool protonect_shutdown = false;
bool displayDepthValue = false;
int clickedX = -1, clickedY = -1;
float pixelValue;
Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, prof, color;

void sigint_handler(int s)
{
  protonect_shutdown = true;
}

void onMouseCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}

int main() {

    cout << "S2" << endl;

    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device* dev = 0;
    libfreenect2::PacketPipeline* pipeline = 0;

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));
    
    if (freenect2.enumerateDevices() == 0)
    {
        cout << "No device connected!" << endl;
        return -1;
    }

    string serial = freenect2.getDefaultDeviceSerialNumber();
    cout << "SERIAL: " << serial << endl;

    if (pipeline)
    {
        dev = freenect2.openDevice(serial, pipeline);
    }
    else
    {
        dev = freenect2.openDevice(serial);
    }

    if (dev == 0)
    {
        cout << "Failure opening device!" << endl;
        return -1;
    }

    menu(dev); 

    signal(SIGINT, sigint_handler);
    protonect_shutdown = false;
     // Initialize OpenCV window for the Kinect stream
    namedWindow("registered");
    setMouseCallback("registered", onMouseCallback);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    zmq::socket_t socket2(context, ZMQ_PUB);
    socket.bind("tcp://0.0.0.0:5555");
    socket2.bind("tcp://0.0.0.0:5556");

    // Initialize the menu
    
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |
                                                  libfreenect2::Frame::Depth |
                                                  libfreenect2::Frame::Ir);
    libfreenect2::FrameMap frames;

    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);

    dev->start();

    cout << "Device serial: " << dev->getSerialNumber() << endl;
    cout << "Device firmware: " << dev->getFirmwareVersion() << endl;
    
    getParams(dev);
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

    while (!protonect_shutdown)
    {   
        listener.waitForNewFrame(frames);
        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];

        Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
        Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
        Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);
        Mat(depth->height, depth->width, CV_8UC2, depth->data).copyTo(prof);
    
        flip(rgbmat, rgbmat, 1); 
		flip(depthmat, depthmat, 1);
        flip(irmat, irmat, 1);

        Mat ROI(rgbmat, Rect(703,327,512,424));
        ROI.copyTo(color);

        auto t1 = high_resolution_clock::now();
  
        registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);

        int r = 0, c = 0;

        while (r < 512 && c < 424) {
            // Extract the x, y, z coordinates and RGB values for the current pixel
            float x, y, z, bgr;
            //uint8_t r, g, b;
            registration->getPointXYZ(&undistorted, /*&registered,*/ r, c, x, y, z/*, bgr*/);
            //uint8_t *p = reinterpret_cast<uint8_t*>(&bgr);
            //cout << bgr << endl;
            //uint8_t b = p[0];
            //uint8_t g = p[1];
            //uint8_t r = p[2];
            //cout << "tamos pana" << endl;
            // Create a point and add it to the cloud
            pcl::PointXYZ point;
            point.x = x;
            point.y = y;
            point.z = z;
            //point.r = r;
            //cout << "red" << r << endl;
            //point.g = g;
            //cout << "green" << g  << endl;
            //point.b = b;
            //cout << "blue" << b  << endl;
            cloud->points.push_back(point);

            r++;
            if (r >= 512) {
                r = 0;
                c++;
            }
        }
        
        auto t2 = high_resolution_clock::now();
        visualizePointCloud(cloud, viewer);
        cloud->clear();
        
        auto ms_int = duration_cast<milliseconds>(t2 - t1);

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;

        cout << ms_int.count() << "ms\n";
        cout << ms_double.count() << "ms\n";

        Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
        Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
        Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);

        if (displayDepthValue)
        {
            if (clickedX >= 0 && clickedY >= 0 && clickedX < depthmat.cols && clickedY < depthmat.rows)
            {
                pixelValue = depthmatUndistorted.at<float>(clickedY, clickedX);
                cout << "Profundidad pixel (" << clickedX << ", " << clickedY << "): " << pixelValue << " mm" << endl;
            }
        }

        libfreenect2::COLOR_SETTING_SET_ANALOG_GAIN;
        
        //putText(rgbd, to_string(pixelValue) + " mm", Point(clickedX, clickedY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(205, 255, 0), 2, LINE_AA);
        imshow("rgb", rgbmat);
        imshow("ir", irmat / 4096.0f);
        imshow("depth", depthmat / 4096.0f);
        //imshow("undistorted", depthmatUndistorted / 4096.0f);
        imshow("registered", rgbd);
        //imshow("depth2RGB", rgbd2 / 4096.0f);
        imshow("cropped", color);
        
        //socket.send(prof.data, prof.total() * prof.elemSize() * prof.channels(), ZMQ_DONTWAIT);
        zmq::message_t message(depth->data, depth->width * depth->height * depth->bytes_per_pixel);
        zmq::message_t message2(rgb->data, rgb->width * rgb->height * rgb->bytes_per_pixel);
        socket.send(message);
        cout << "depth sent" << endl;
        socket2.send(message);
        cout << "color sent" << endl;
        //thread sendColor(socketsend, color);
        //this_thread::sleep_for(20ms);
        
        int key = waitKey(1);
        protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));

        listener.release(frames);
    }

    dev->stop();
    dev->close();
    delete registration;

    cout << "Streaming Ends!" << endl;
    return 0;
}
