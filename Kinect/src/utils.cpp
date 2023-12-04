#include "../include/utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <zmq.hpp>
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
    socket->bind(serverAddress);
}

zmq_stream::~zmq_stream() {
    // Cleanup
    socket->close();
    delete socket;
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


void readIni() {

    std::ifstream infile("../config.ini");
    if (infile.is_open()) {
        std::string line;
        std::string currentSection; // Track the current section

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string key, value, section;

            // Check for a section
            if (line.find('[') != std::string::npos && line.find(']') != std::string::npos) {
                // Extract section name
                section = line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1);
                currentSection = section;
            } else {
                // No section, parse key-value pair
                if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                    // Trim leading and trailing whitespaces
                    key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));
                    key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
                    value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
                    value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

                    // Literalmente no encontre otra forma mas eficiente para comparar el ini aiuda

                    if (currentSection == "ColorCameraParams") {
                        try {
                            if (key == "fx") {
                                    ColorCameraParams.fx = std::stod(value);
                            }
                            else if (key == "fy") {
                                    ColorCameraParams.fy = std::stod(value);
                            }
                            else if (key == "cx") {
                                    ColorCameraParams.cx = std::stod(value);
                            }
                            else if (key == "cy") {
                                    ColorCameraParams.cy = std::stod(value);
                            }
                        } catch(const std::invalid_argument& e) {
                            std::cerr << "Error en el parseo de datos" << std::endl;
                        }
                    } 
                    else if (currentSection == "IrCameraParams") {
                        try {
                            if (key == "fx") {
                                    IrCameraParams.fx = std::stod(value);
                            }
                            else if (key == "fy") {
                                    IrCameraParams.fy = std::stod(value);
                            }
                            else if (key == "cx") {
                                    IrCameraParams.cx = std::stod(value);
                            }
                            else if (key == "cy") {
                                    IrCameraParams.cy = std::stod(value);
                            }
                            else if (key == "k1") {
                                    IrCameraParams.k1 = std::stod(value);
                            }
                            else if (key == "k2") {
                                    IrCameraParams.k2 = std::stod(value);
                            }
                            else if (key == "k3") {
                                    IrCameraParams.k3 = std::stod(value);
                            }
                            else if (key == "p1") {
                                    IrCameraParams.p1 = std::stod(value);
                            }
                            else if (key == "p2") {
                                    IrCameraParams.p2 = std::stod(value);
                            }
                        } catch(const std::invalid_argument& e) {
                            std::cerr << "Error en el parseo de datos" << std::endl;
                        }
                    }
                }
            }
        }
        infile.close();
    } else {
        std::cerr << "No es posible abrir config.ini" << std::endl;
    }
}

void writeIni() {

    // Write updated parameters to INI file
    std::ofstream outfile("../config.ini");
    if (outfile.is_open()) {
        outfile << "[ColorCameraParams]" << std::endl;
        outfile << "fx = " << ColorCameraParams.fx << std::endl;
        outfile << "fy = " << ColorCameraParams.fy << std::endl;
        outfile << "cx = " << ColorCameraParams.cx << std::endl;
        outfile << "cy = " << ColorCameraParams.cy << std::endl;
        outfile << "" << std::endl;
        outfile << "[IrCameraParams]" << std::endl;
        outfile << "fx = " << IrCameraParams.fx << std::endl;
        outfile << "fy = " << IrCameraParams.fy << std::endl;
        outfile << "cx = " << IrCameraParams.cx << std::endl;
        outfile << "cy = " << IrCameraParams.cy << std::endl;
        outfile << "k1 = " << IrCameraParams.k1 << std::endl;
        outfile << "k2 = " << IrCameraParams.k2 << std::endl;
        outfile << "k3 = " << IrCameraParams.k3 << std::endl;
        outfile << "p1 = " << IrCameraParams.p1 << std::endl;
        outfile << "p2 = " << IrCameraParams.p2 << std::endl;
        outfile.close();
        std::cout << "Parametros escritos en config.ini" << std::endl;
    } else {
        std::cerr << "No se puede abrir config.ini" << std::endl;
    }
}

void getParams(libfreenect2::Freenect2Device *dev) {

    ColorCameraParams = dev->getColorCameraParams();
    IrCameraParams = dev->getIrCameraParams();
}   

void setParams(libfreenect2::Freenect2Device *dev) {

    dev->setColorCameraParams(ColorCameraParams);
    dev->setIrCameraParams(IrCameraParams);
}

