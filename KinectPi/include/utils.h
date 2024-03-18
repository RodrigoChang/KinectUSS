#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include <opencv2/opencv.hpp>
#include <zmq.hpp>
#include <string>
#include <chrono>

//extern libfreenect2::Registration* registration;
extern libfreenect2::Freenect2 freenect2;
extern libfreenect2::Freenect2Device *dev;
extern libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
extern libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
extern libfreenect2::Freenect2Device::Config config;
extern bool onStreaming, protonect_shutdown, enable_rgb, enable_ogl, enable_viewer, enable_depth, enable_stream, enable_cloud, enable_z_stream;
extern std::string ip;
extern std::chrono::milliseconds frametime;
//ZMQ
class zmq_stream {
private:
    zmq::socket_t* socket;
    std::string serverPort;
    std::string serverAddress;
    int socketType;

public:
    zmq_stream(const std::string& serverAddress, const std::string& serverPort, int socketType);
    ~zmq_stream();

    zmq::message_t receive();
    void send_msg(std::string msg);
    void encodeo_envio(const cv::Mat& frame);
    void envio_plain(const cv::Mat& frame);
    

private:
    std::vector<uchar> encodeo(const cv::Mat& frame);
    static zmq::context_t context;
};

//Kinect
void kinect(std::string serial);

//utils.cpp
std::map<std::string, std::string> readConfigFile(const std::string& filename);
void getParams(libfreenect2::Freenect2Device *dev);
void setParams(libfreenect2::Freenect2Device *dev);
int confirmacion();
//depth menu
void menu();
