// Este es el kinect.cpp, aqui stremeamos
#include <iostream>
#include <string>
#include <sstream>
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
#include <mutex>

using namespace std;

bool displayDepthValue = false;
int clickedX = -1, clickedY = -1;
float pixelValue;
mutex stream_mutex;
thread mat1, mat2, recorte, recv_mesg, cloud_thread;
cv::Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, cropped, resized, reg_send, ir_send, depth_send;

libfreenect2::Registration* registration;
libfreenect2::PacketPipeline* pipeline = 0;
libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

static void onMouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}

static void find_z() {
    zmq_stream depth_points(ip, "5557", ZMQ_REP);
    while(!protonect_shutdown) {
        zmq::message_t request;
        request = depth_points.receive();
        string receivedMsg = string(static_cast<char*>(request.data()), request.size());
        receivedMsg.erase(receivedMsg.size() - 1);
        cout << "Data: " << receivedMsg << endl;
        istringstream ss(receivedMsg);
        vector<int> vect;
        string respuesta;

        for (int i; ss >> i;) {
            vect.push_back(i);    
            if (ss.peek() == ',') ss.ignore();
        }

        if(vect.size() != 32) {
            cout << "z invalido, recivido un string con tamaño " << vect.size() << endl;
            depth_points.send_msg("z invalido, recivido un string con tamaño " + vect.size());
            continue;
        }

        for(size_t i = 0; i < vect.size(); i+=2) {
            int x = vect[i];
            int y = vect[i + 1];
            if (x >= 0 && y >= 0 && x < depth_send.cols && y < depth_send.rows) {
                pixelValue = depth_send.at<float>(y, x);
                respuesta += "," + to_string(pixelValue);
            }
            else respuesta += ",0";
        }
        
        respuesta.erase(0, 1);
        depth_points.send_msg(respuesta);
        cout << "Data final: " << respuesta << endl;
        this_thread::sleep_for(frametime);
    }
}

static void zmq_streaming() {
    zmq_stream rgb_stream(ip, "5555", ZMQ_PUB);
    zmq_stream ir_stream(ip, "5556", ZMQ_PUB);;
    zmq_stream registered_stream(ip, "5558", ZMQ_PUB);
    cout << "socket listos" << endl;
    this_thread::sleep_for(chrono::milliseconds(5000));
    cout << "comenzando stream" << endl;
    while(!protonect_shutdown) {
        if (recorte.joinable()) recorte.join();
        thread rgb_streaming([&rgb_stream] () {rgb_stream.encodeo_envio(resized);});
        //lock_guard<mutex> guard(stream_mutex);
        if(mat1.joinable()) mat1.join();
        thread ir_streaming([&ir_stream] () {ir_stream.encodeo_envio(ir_send / 512.0f );});
        if(mat2.joinable()) mat2.join();
        thread registered_streaming([&registered_stream] () {registered_stream.encodeo_envio(reg_send);});
        rgb_streaming.join();
        ir_streaming.join();
        registered_streaming.join();
        this_thread::sleep_for(frametime);
        if (protonect_shutdown) break;
    }
}

void kinect(string serial) {
    cout << "Iniciando Kinect default" << endl;
    //Abriendo la kiect basado en el n serie
    if(enable_ogl) pipeline = new libfreenect2::OpenGLPacketPipeline();
    else pipeline = new libfreenect2::CpuPacketPipeline();
    //Abriendo la kiect basado en el n serial 
    if (pipeline) dev = freenect2.openDevice(serial, pipeline);
    else dev = freenect2.openDevice(serial);      
    dev = freenect2.openDevice(serial);
    if (dev == 0) {
        cout << "No se pudo abrir la kinect" << endl;
        int conf = confirmacion();
        if(!conf) onStreaming = false;
    }       
    // Opencv value thing
    if(enable_viewer) {
        cv::namedWindow("registered");
        cv::setMouseCallback("registered", onMouseCallback);
    }
    cout << "Abriendo Menu" << endl;
    if(enable_viewer) menu(); //Inicilizamos el menu 
    //seteando el listener de libfreenect2
    int types = 0;
    if (enable_rgb) types |= libfreenect2::Frame::Color;
    if (enable_depth) types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
    libfreenect2::SyncMultiFrameListener listener(types);
    libfreenect2::FrameMap frames;
    thread frame_streaming;
    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);
    dev->start();
    cout << "No de serie: " << dev->getSerialNumber() << endl;
    cout << "Firmware de la Kinect : " << dev->getFirmwareVersion() << endl;
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

    if (enable_stream) frame_streaming = thread(zmq_streaming);
    if (enable_z_stream) recv_mesg = thread(find_z);
 
    //if (enable_cloud) thread cloud_function(cloud_streaming);
    while (!protonect_shutdown) {   //Mientras spawneen mas frames va a seguir ejecutandose
        listener.waitForNewFrame(frames);
        libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];
        //registration
        registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);

        thread mat1([&rgb, &ir, &depth] () {
            //Creamos las matrices para poder visualizarl los streams
            cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
            cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
            cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);
            cv::flip(rgbmat, rgbmat, 1); 
            cv::flip(depthmat, depth_send, 1);
            cv::flip(irmat, ir_send, 1);
        });

        thread mat2([&undistorted, &registered, &depth2rgb] () {
            //Creamos las matrices para poder visualizarl los streams
            cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
            cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
            cv::Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);
            cv::flip(depthmatUndistorted, depthmatUndistorted, 1); 
            cv::flip(rgbd, reg_send, 1);
            cv::flip(rgbd2, rgbd2, 1);
        });
        
        mat1.join();

        thread recorte([] () {
            cv::Mat ROI(rgbmat, cv::Rect(308,0,1304,1080));
            ROI.copyTo(cropped);
            cv::resize(cropped, resized, cv::Size(512, 424), 0, 0, cv::INTER_LINEAR);
            //Creamos la imagen recortada para hacer fit al mediapipe
        });

        //Display de profundidad ***TO DO*** Hacer algo mas bonito
        if (displayDepthValue) {
            if (clickedX >= 0 && clickedY >= 0 && clickedX < depth_send.cols && clickedY < depth_send.rows) {
                pixelValue = depth_send.at<float>(clickedY, clickedX);
                cout << "Profundidad pixel (" << clickedX << ", " << clickedY << "): " << pixelValue << " mm" << endl;
                displayDepthValue = false;
            }
        }
        
        mat2.join();
        recorte.join();
        
        if (enable_viewer) {
            cv::putText(reg_send, to_string(pixelValue) + " mm", cv::Point(clickedX, clickedY), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(205, 255, 0), 2, cv::LINE_AA);
            //imshow("rgb", rgbmat);
            //imshow("ir", irmat / 4096.0f);
            //imshow("depth", depthmat / 4096.0f);
            //imshow("undistorted", depthmatUndistorted / 4096.0f);
            cv::imshow("registered", reg_send);
            //imshow("depth2RGB", rgbd2 / 4096.0f);
            //imshow("cropped", cropped);
        }   
        int key = cv::waitKey(1);
        protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));
        listener.release(frames);
    }
    cout << "Deteniendo Kinect" << endl;
    dev->stop();
    dev->close();
    delete registration;
    cv::destroyAllWindows();
    onStreaming = false;
}

