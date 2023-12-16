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
using namespace cv;

bool displayDepthValue = false;
int clickedX = -1, clickedY = -1;
float pixelValue;
mutex stream_mutex;
thread mat1, mat2, recorte, recv_mesg, cloud_thread;
Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, cropped, resized, reg_send, ir_send, depth_send;

libfreenect2::Registration* registration;
libfreenect2::PacketPipeline* pipeline = 0;
libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);

static void onMouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}

static void cloud_streaming() {
    PointCloud pCloud;
    this_thread::sleep_for(chrono::milliseconds(5000));
    cout << "comenzando cloud" << endl;
    if (tipo_cloud = PointCloud::GRAY) {
        while(!protonect_shutdown) {
            pCloud.getPointCloud(registration, &undistorted, PointCloud::GRAY);
            pCloud.visualizePointCloud();
            this_thread::sleep_for(frametime);
            if (protonect_shutdown) break;    
        }
    }

    else if (tipo_cloud = PointCloud::RGB) {
        while(!protonect_shutdown) {
            pCloud.getPointCloud(registration, &undistorted, PointCloud::RGB, &registered);
            pCloud.visualizePointCloudRGB();
            this_thread::sleep_for(frametime);
            if (protonect_shutdown) break;
        }
    }
    else if (tipo_cloud = PointCloud::RGB2) {
        while(!protonect_shutdown) {
            pCloud.getPointCloud(registration, &undistorted, PointCloud::RGB2, &registered);
            pCloud.visualizePointCloudRGB();
            this_thread::sleep_for(frametime);
            if (protonect_shutdown) break;
        }
    }
}

static void find_z() {
    zmq_stream depth_points(ip, "5557", ZMQ_REP);
    while(!protonect_shutdown) {
        zmq::message_t request;
        request = depth_points.receive();
        string receivedMsg = string(static_cast<char*>(request.data()), request.size());
        receivedMsg.erase(receivedMsg.size() - 1);
        cout << "Data:" << receivedMsg << endl;
        std::istringstream ss(receivedMsg);
        std::string token;
        std::vector<int> par;

        while (getline(ss, token, ',')) {
            int value = std::stoi(token);
            par.push_back(value);
        }

        cout << par.size() << endl;
        string resp_iz, resp_der, respuesta;
        //iss >> x >> coma >> y;
        thread z_izquierdo([&par, &resp_iz] () {
        int parSize = par.size();
        for(int i = 0; i < 16; i =+ 2) {
            int x = par[i];
            int y = par[i + 1];
            if (x >= 0 && y >= 0 && x < depth_send.cols && y < depth_send.rows) {
                pixelValue = depth_send.at<float>(y, x);
                resp_iz += "," + to_string(pixelValue);
            }
            else resp_iz += ",0";
        }
    });

    thread z_derecho([&par, &resp_der] () {
        int parSize = par.size();
        for(int i = 8; i < 32; i =+ 2) {
            int x = par[i];
            int y = par[i + 1];
            if (x >= 0 && y >= 0 && x < depth_send.cols && y < depth_send.rows) {
                pixelValue = depth_send.at<float>(y, x);
                resp_der += "," + to_string(pixelValue);
            }
            else resp_der += ",0";
        }
    });

    z_izquierdo.join();
    z_derecho.join();

    cout << "izquierda" << resp_iz << endl;
    cout << "derecha" << resp_der << endl;
    respuesta.erase(0, 1);
    depth_points.send_msg(respuesta);
    cout << "final" << respuesta << endl;
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
    pipeline = new libfreenect2::OpenGLPacketPipeline();
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
    namedWindow("registered");
    setMouseCallback("registered", onMouseCallback);
    cout << "Abriendo Menu" << endl;
    menu(); //Inicilizamos el menu 
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
    if (enable_cloud) cloud_thread = thread(cloud_streaming);
 
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
            thread rgb_mat([&rgb] () {Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);});
            thread ir_mat([&ir] () {Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);});
            thread depth_mat([&depth] () {Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);});
            rgb_mat.join();
            flip(rgbmat, rgbmat, 1); 
            //por alguna razon los frames directo de la kinect salen en mirror, asi que aqui los damos vuelta
            ir_mat.join();
            depth_mat.join();
            flip(depthmat, depth_send, 1);
            flip(irmat, ir_send, 1);
        });

        thread mat2([&undistorted, &registered, &depth2rgb] () {
            //Creamos las matrices para poder visualizarl los streams
            thread undistorted_mat([&undistorted] () {Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);});
            thread registered_mat([&registered] () {Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);});
            thread depth2rgb_mat([&depth2rgb] () { Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);});
            undistorted_mat.join();
            registered_mat.join();
            depth2rgb_mat.join();
            flip(depthmatUndistorted, depthmatUndistorted, 1); 
            flip(rgbd, reg_send, 1);
            flip(rgbd2, rgbd2, 1);
        });
        
        mat1.join();

        thread recorte([] () {
            Mat ROI(rgbmat, Rect(308,0,1304,1080));
            ROI.copyTo(cropped);
            resize(cropped, resized, Size(512, 424), 0, 0, INTER_LINEAR);
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
        recorte.join();
        mat2.join();

        cv::putText(reg_send, to_string(pixelValue) + " mm", Point(clickedX, clickedY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(205, 255, 0), 2, LINE_AA);
        //imshow("rgb", rgbmat);
        //imshow("ir", irmat / 4096.0f);
        //imshow("depth", depthmat / 4096.0f);
        //imshow("undistorted", depthmatUndistorted / 4096.0f);
        cv::imshow("registered", reg_send);
        //imshow("depth2RGB", rgbd2 / 4096.0f);
        //imshow("cropped", cropped);
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

