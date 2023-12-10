#include "../include/utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>
#include <C:/Users/rodri/Documents/Git/vcpkg/packages/zeromq_x64-windows/include/zmq.hpp>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>

using namespace cv;

/* 
Clase zmq_stream
Esta clase stremea a traves de zmq en el address y puerto elegido
puede encodear o no el frame si es necesario mantener, o es posible optimizar la data.
Es mucho mejor llevarlo a clase, ya que asi solucionamos los problemas de ownership, y tal.
A y tambien multithreading si es necesario encodear.
xd
*/
zmq::context_t zmq_stream::context(1);

zmq_stream::zmq_stream(const std::string& serverAddress, int socketType)
    : serverAddress(serverAddress), socketType(socketType) {
    socket = new zmq::socket_t(context, socketType);

    // Connecting
    socket->connect(serverAddress);
}

zmq_stream::~zmq_stream() {
    // Cleanup
    socket->close();
    delete socket;
}

void zmq_stream::send_mgs(std::string msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.data(), msg.size());
    socket->send(message);
}

zmq::message_t zmq_stream::receive() {
    zmq::message_t message;
    socket->recv(message);
    return message;
}

void zmq_stream::encodeo_envio(const Mat& frame) {
    std::vector<uchar> encodedframe = encodeo(frame);
    // mempcpy moment
    zmq::message_t message(encodedframe.size());
    memcpy(message.data(), encodedframe.data(), encodedframe.size());
    // envio
    socket->send(message);
    //std::cout << "Frame encodeado enviado" << std::endl;
}

void zmq_stream::envio_plain(const Mat& frame) {
    zmq::message_t message(frame.total() * frame.elemSize());
    memcpy(message.data(), frame.data, message.size());
    socket->send(message);
    //std::cout << "Frame enviado" << std::endl;
}

std::vector<uchar> zmq_stream::encodeo(const Mat& frame) {
    std::vector<uchar> encodedFrame;
    cv::imencode(".jpg", frame, encodedFrame);
    return encodedFrame;
}

int confirmacion() {
    int numb;
    while (true) {
        char resp;
        std::cout << "Intentar de nuevo? [S/n]" << std::endl;
        std::cin >> resp;
            if(resp == 's' || resp == 'S') {
                numb = 1;
                break;
            }
            else if (resp == 'n' || resp == 'N') {
                numb = 0;
                break;
            }
    }
    return numb;
}

std::map<std::string, std::string> readConfigFile(const std::string& filename) {
    std::map<std::string, std::string> config;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse key-value pairs
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Trim leading and trailing whitespaces
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Store key-value pairs in the map
            config[key] = value;
        }
    }

    return config;
}

void getParams(libfreenect2::Freenect2Device *dev) {

    ColorCameraParams = dev->getColorCameraParams();
    IrCameraParams = dev->getIrCameraParams();
}   

void setParams(libfreenect2::Freenect2Device *dev) {

    dev->setColorCameraParams(ColorCameraParams);
    dev->setIrCameraParams(IrCameraParams);
}

