// Este es el kinect.cpp, aqui stremeamos
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "../include/utils.h"
#include "../include/log.h"
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

bool displayDepthValue = false;
int clickedX = -1, clickedY = -1;
float pixelValue;
auto frametime = milliseconds(16);
Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, cropped;

libfreenect2::PacketPipeline* pipeline = 0;

void onMouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}

void kinect(string serial) {
    cout << "Iniciando Kinect default" << endl;
    //Abriendo la kiect basado en el n serie
    pipeline = new libfreenect2::OpenGLPacketPipeline(); 
    if (pipeline) dev = freenect2.openDevice(serial, pipeline);
    else dev = freenect2.openDevice(serial);      
    dev = freenect2.openDevice(serial);
    if (dev == 0) {
        cout << "No se pudo abrir la kinect" << endl;
        int conf = confirmacion();
        if(!conf) onStreaming = false;
    }       
    //Abriendo la kiect basado en el n serial
    thread zmq_streaming;
    // Initialize OpenCV window for the Kinect stream
    namedWindow("registered");
    setMouseCallback("registered", onMouseCallback);

    zmq_stream rgb_stream("tcp://localhost:5555", ZMQ_PUB);
    zmq_stream ir_stream("tcp://localhost:5556", ZMQ_PUB);
    zmq_stream depth_stream("tcp://localhost:5557", ZMQ_PUB);
    zmq_stream registered_stream("tcp://localhost:5558", ZMQ_PUB);
    //PointCloud point_cloud_basico;

    //seteando el listener de libfreenect2
    int types = 0;
    if (enable_rgb) types |= libfreenect2::Frame::Color;
    if (enable_depth) types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    libfreenect2::SyncMultiFrameListener listener(types);
    libfreenect2::FrameMap frames;

    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);
    dev->start();
    cout << "No de serie: " << dev->getSerialNumber() << endl;
    cout << "Firmware de la Kinect : " << dev->getFirmwareVersion() << endl;
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
    auto frame1 = high_resolution_clock::now();

    while (!protonect_shutdown) {   //Mientras spawneen mas frames va a seguir ejecutandose
        listener.waitForNewFrame(frames);
        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];
        //registration
        thread reg_apply([&registration, &rgb, &depth, &undistorted, &registered, &depth2rgb] () {registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);});
        
        thread mat1([&rgb, &ir, &depth] () {
            //Creamos las matrices para poder visualizarl los streams
            Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
            Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
            Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);
        });
        
        //point cloud
        //point_cloud_basico.getPointCloud(registration, &undistorted);
       // point_cloud_basico.visualizePointCloud();
        reg_apply.join();
        thread mat2([&undistorted, &registered, &depth2rgb] () {
            //Creamos las matrices para poder visualizarl los streams
            Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
            Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
            Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);
        });

        //por alguna razon los frames directo de la kinect salen en mirror, asi que aqui los damos vuelta
        mat1.join();
        flip(rgbmat, rgbmat, 1); 
        flip(depthmat, depthmat, 1);
        flip(irmat, irmat, 1);
        mat2.join();

        //Display de profundidad ***TO DO*** Hacer algo mas bonito
        if (displayDepthValue) {
            if (clickedX >= 0 && clickedY >= 0 && clickedX < depthmat.cols && clickedY < depthmat.rows) {
                pixelValue = depthmatUndistorted.at<float>(clickedY, clickedX);
                cout << "Profundidad pixel (" << clickedX << ", " << clickedY << "): " << pixelValue << " mm" << endl;
            }
        }

        thread thread_show([] () {
            //Creamos la imagen recortada para hacer fit al mediapipe
            Mat ROI(rgbmat, Rect(308,0,1304,1080));
            ROI.copyTo(cropped);
            flip(depthmatUndistorted, depthmatUndistorted, 1); 
            flip(rgbd, rgbd, 1);
            flip(rgbd2, rgbd2, 1);
            putText(rgbd, to_string(pixelValue) + " mm", Point(clickedX, clickedY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(205, 255, 0), 2, LINE_AA);
            //imshow("rgb", rgbmat);
            //imshow("ir", irmat / 4096.0f);
            imshow("depth", depthmat / 4096.0f);
            //imshow("undistorted", depthmatUndistorted / 4096.0f);
            imshow("registered", rgbd);
            //imshow("depth2RGB", rgbd2 / 4096.0f);
            imshow("cropped", cropped);
        });

        //streaming thread si llego a los 30 frames
        auto frame2 = high_resolution_clock::now();
        if (duration_cast<milliseconds>(frame2 - frame1) >= frametime) {
            
            thread zmq_streaming([&rgb_stream, &ir_stream, &depth_stream, &registered_stream](){
                rgb_stream.encodeo_envio(cropped);
                ir_stream.envio_plain(irmat);
                depth_stream.envio_plain(depthmat);
                registered_stream.encodeo_envio(rgbd);
            });
            auto frame1 = high_resolution_clock::now();
        }
        
        int key = waitKey(1);
        protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));
        listener.release(frames);
        thread_show.join();
        if (zmq_streaming.joinable()) {
            zmq_streaming.join();  
        }
    }
    cout << "Deteniendo Kinect" << endl;
    dev->stop();
    dev->close();
    delete registration;
    destroyAllWindows();
    onStreaming = false;
}

