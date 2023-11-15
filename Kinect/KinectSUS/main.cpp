#include <csignal>
#include <iostream>
#include <string>
#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "camera_params.h"
#include "menu.h"


using namespace std;
using namespace cv;
ColorCameraParams colorParams; 
Config depthParams;
IrCameraParams irParams;

bool protonect_shutdown = false;
bool displayDepthValue = false;
int clickedX = -1, clickedY = -1;
float pixelValue;
Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, prof;

void sigint_handler(int s)
{
  protonect_shutdown = true;
}

void onMouseCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}


int main()
{
    cout << "S2" << endl;

    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device* dev = 0;
    libfreenect2::PacketPipeline* pipeline = 0;

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

    signal(SIGINT, sigint_handler);
    protonect_shutdown = false;

    // Use the parameters from configuration classes
    //libfreenect2::Registration* registration = new libfreenect2::Registration(
    //    irParams.fx, irParams.fy, irParams.cx, irParams.cy,
    //    irParams.k1, irParams.k2, irParams.k3, irParams.p1, irParams.p2,
    //    colorParams.fx, colorParams.fy, colorParams.cx, colorParams.cy
    //);

     // Initialize OpenCV window for the Kinect stream
    namedWindow("registered");
    setMouseCallback("registered", onMouseCallback);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.bind("tcp://0.0.0.0:5555");

    // Initialize the menu
    menu(); // Run the menu application
    
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |
                                                  libfreenect2::Frame::Depth |
                                                  libfreenect2::Frame::Ir);
    libfreenect2::FrameMap frames;

    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);

    dev->start();

    cout << "Device serial: " << dev->getSerialNumber() << endl;
    cout << "Device firmware: " << dev->getFirmwareVersion() << endl;
    
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);


    while (!protonect_shutdown)
    {   
        listener.waitForNewFrame(frames);
        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];

        cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
        cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
        cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);

        registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);

        cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
        cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
        cv::Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);

        if (displayDepthValue)
        {
            if (clickedX >= 0 && clickedY >= 0 && clickedX < depthmat.cols && clickedY < depthmat.rows)
            {
                pixelValue = depthmatUndistorted.at<float>(clickedY, clickedX);
                cout << "Profundidad pixel (" << clickedX << ", " << clickedY << "): " << pixelValue << " mm" << endl;
                
                // Reset the flag and coordinates
            }
        }

        putText(rgbd, to_string(pixelValue) + " mm", Point(clickedX, clickedY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(205, 255, 0), 2, LINE_AA);

        imshow("rgb", rgbmat);
        imshow("ir", irmat / 4096.0f);
        imshow("depth", depthmat / 4096.0f);
        imshow("undistorted", depthmatUndistorted / 4096.0f);
        imshow("registered", rgbd);
        imshow("depth2RGB", rgbd2 / 4096.0f);

            // Reading the image through opencv package
        cv::Mat(depth->height, depth->width, CV_8UC2, depth->data).copyTo(prof);
        socket.send(prof.data, prof.total() * prof.elemSize() * prof.channels());
        //std::vector<uchar> buffer; , zmq::send_flags::none
       // cv::imencode(".png", prof, buffer);
        //zmq::message_t message(buffer.size());
        //memcpy(message.data(), buffer.data(), buffer.size());
        //socket.send(message, zmq::send_flags::none);

        int key = cv::waitKey(1);
        protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));

        listener.release(frames);
    }

    dev->stop();
    dev->close();
    delete registration;

    cout << "Streaming Ends!" << endl;
    return 0;
}
