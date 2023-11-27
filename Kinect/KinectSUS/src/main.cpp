/*
Kinect USS
Este es el archivo principal de KinectSUS
Consiste en hacer stream de rgb, ir y depth a traves de libfreenect2
luego transforma los frames en matrices de opencv para poder visualizarlas.
Tambien crea el viewer de point cloud, y registra xyz para el mismo.
Actualmente entrega el frame de rgb y depth a traves de una transmision de opencv
*/ 

#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <zmq.hpp>
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "../include/utils.h"
#include "../include/log.h"
#include "../include/menu.h"
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

bool protonect_shutdown = false, displayDepthValue = false;
bool inMenu = true, enable_rgb = true, enable_depth = true, enable_stream = true;
int clickedX = -1, clickedY = -1;
float pixelValue;
Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2, cropped;

zmq::context_t context(1);
zmq::socket_t rgb_socket(context, ZMQ_PUB);
zmq::socket_t ir_socket(context, ZMQ_PUB);
zmq::socket_t depth_socket(context, ZMQ_PUB);
zmq::socket_t registered_socket(context, ZMQ_PUB);

libfreenect2::Freenect2 freenect2;
libfreenect2::Freenect2Device *dev = 0;
libfreenect2::PacketPipeline *pipeline = 0;

void menu();
void mainMenu();
void sigint_handler(int s) {
    protonect_shutdown = true;
}

void onMouseCallback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        clickedX = x;
        clickedY = y;
        displayDepthValue = true;
    }
}

void socket_intit() {
    //zmq, context de 1 io_thread, 4 sockets
    cout << "Inicializando sockets" << endl;
    rgb_socket.bind("tcp://0.0.0.0:5555");
    ir_socket.bind("tcp://0.0.0.0:5556");
    depth_socket.bind("tcp://0.0.0.0:5557");
    registered_socket.bind("tcp://0.0.0.0:5558");
    cout << "Sockets listos" << endl;
}

int confirmacion() {
    int numb;
    while (true) {
        char resp;
        cout << "Intentar de nuevo? [S/n]" << endl;
        cin >> resp;
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

bool kinectSearch() {
    cout << "Buscando kinect" << endl;
    if (freenect2.enumerateDevices() == 0) {
        cout << "Conecta la kinect po!" << endl;
        return false;
    }
    else {
        cout << "Kinect encontrada" << endl;
        return true;
    }
}

int main(int argc, char *argv[]) {

    /*QApplication app(argc, argv);
    MainMenu mainMenu;
    mainMenu.setWindowTitle("Menu Principal");
    mainMenu.show();
    return app.exec();
    string program_path(argv[0]);*/

    //Argumentos
    cerr << "Argumentos: [-norgb | -nodepth | -nostream] " << endl;
    //File logger
    MyFileLogger *filelogger = new MyFileLogger(getenv("LOGFILE"));
    if (filelogger->good()) libfreenect2::setGlobalLogger(filelogger);
    else delete filelogger;

    for(int argI = 1; argI < argc; ++argI) {
        string arg(argv[argI]);
        if(arg == "-norgb" || arg == "--norgb") enable_rgb = false;
        else if(arg == "-nodepth" || arg == "--nodepth") enable_depth = false;
        else if(arg == "-nostream" || arg == "--nostream") enable_stream = false;
    }
    if (!enable_rgb && !enable_depth) {
        cerr << "toncs que stremeamos?" << endl;
        return -1;
    }

    cout << "Inicializando sistema" << endl;
    //libfreenect values
    signal(SIGINT, sigint_handler); // handler para ctrl + c
    while(inMenu) {
        bool found = kinectSearch();
        if (!found) {
            int conf = confirmacion();
            if(!conf) return 0;
            else continue;
        }
        else {
            string serial = freenect2.getDefaultDeviceSerialNumber();
            cout << "Iniciando Kinect default" << endl;
            //Abriendo la kiect basado en el n serie
            pipeline = new libfreenect2::CpuPacketPipeline();
            if (pipeline) dev = freenect2.openDevice(serial, pipeline);
            else dev = freenect2.openDevice(serial);      
            if (dev == 0) {
                cout << "No se pudo abrir la kinect" << endl;
                int conf = confirmacion();
                if(!conf) return 0;
                else continue;
            }       
            cout << "No de serie: " << dev->getSerialNumber() << endl;
            cout << "Firmware de la Kinect : " << dev->getFirmwareVersion() << endl;
            mainMenu();
            cout << "Abriendo Menu" << endl;
            menu(); //Inicilizamos el menu
            if (enable_stream) socket_intit(); //seteando el socket
            //seteando el listener de libfreenect2
            int types = 0;
            if (enable_rgb) types |= libfreenect2::Frame::Color;
            if (enable_depth) types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
            libfreenect2::SyncMultiFrameListener listener(types);
            libfreenect2::FrameMap frames;
            dev->setColorFrameListener(&listener);
            dev->setIrAndDepthFrameListener(&listener);
            libfreenect2::Registration* registration = new libfreenect2::Registration(IrCameraParams, dev->getColorCameraParams());
            libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
            cout << "Iniciando transmision" << endl;
            dev->start();
            while (!protonect_shutdown)
            {   //Mientras spawneen mas frames va a seguir ejecutandose, ***TO DO***  Buscar una manera de limitarlos?
                listener.waitForNewFrame(frames);
                libfreenect2::Frame* rgb = frames[libfreenect2::Frame::Color];
                libfreenect2::Frame* ir = frames[libfreenect2::Frame::Ir];
                libfreenect2::Frame* depth = frames[libfreenect2::Frame::Depth];
                //Creamos las matrices para poder visualizarl los streams
                Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
                Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
                Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);
                //por alguna razon los frames directo de la kinect salen en mirror, asi que aqui los damos vuelta
                flip(rgbmat, rgbmat, 1); 
                flip(depthmat, depthmat, 1);
                flip(irmat, irmat, 1);
                //Creamos la imagen recortada para hacer fit al mediapipe
                Mat ROI(rgbmat, Rect(308,0,1304,1080));
                ROI.copyTo(cropped);

                registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);
                //point cloud
                getCloudData(registration, &undistorted);
                visualizePointCloud();
                //Matrices para ver los frames de la registration
                Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
                Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
                Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);
                flip(depthmatUndistorted, depthmatUndistorted, 1); 
                flip(rgbd, rgbd, 1);
                flip(rgbd2, rgbd2, 1);
                //Display de profundidad ***TO DO*** Hacer algo mas bonito
                if (displayDepthValue) {
                    if (clickedX >= 0 && clickedY >= 0 && clickedX < depthmat.cols && clickedY < depthmat.rows) {
                        pixelValue = depthmatUndistorted.at<float>(clickedY, clickedX);
                        cout << "Profundidad pixel (" << clickedX << ", " << clickedY << "): " << pixelValue << " mm" << endl;
                    }
                }
                
                putText(rgbd, to_string(pixelValue) + " mm", Point(clickedX, clickedY), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(205, 255, 0), 2, LINE_AA);
                //imshow("rgb", rgbmat);
                imshow("ir", irmat / 4096.0f);
                imshow("depth", depthmat / 4096.0f);
                //imshow("undistorted", depthmatUndistorted / 4096.0f);
                //imshow("registered", rgbd);
                //imshow("depth2RGB", rgbd2 / 4096.0f);
                imshow("cropped", cropped);

                auto t1 = high_resolution_clock::now();
                /*
                thread sendColor(send_zmq, make_tuple(cropped, rgb_socket, true, "rgb"));
                thread sendIr(send_zmq, make_tuple(irmat, ir_socket, true, "ir"));
                thread sendDepth(send_zmq, make_tuple(depthmat, depth_socket, false, "depth"));
                thread sendRegistered(send_zmq, registered, registered_socket, false, "registered");
                */
                //momento parece que no es thread safe el zmq xd
                if (enable_stream) {
                    auto t1 = high_resolution_clock::now();
                    send_zmq(cropped, move(rgb_socket), true, "rgb");
                    send_zmq(irmat, move(ir_socket), false, "ir");
                    send_zmq(depthmat, move(depth_socket), false, "depth");
                    send_zmq(rgbd, move(registered_socket), true, "registered");
                    auto t2 = high_resolution_clock::now();
                    auto ms_int = duration_cast<milliseconds>(t2 - t1);
                cout << ms_int.count() << "ms\n";
                }
                
                auto t2 = high_resolution_clock::now();
                //this_thread::sleep_for(20ms);
                
                int key = waitKey(1);
                protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));

                listener.release(frames);

                auto ms_int = duration_cast<milliseconds>(t2 - t1);
                cout << ms_int.count() << "ms\n";
            }
            cout << "Deteniendo Kinect" << endl;
            dev->stop();
            dev->close();
            delete registration;
            //Deleteamos los sockets, y nos piteamos las windows para hacer un cierre mas bonito
            if (enable_stream) {
                zmq_close(rgb_socket);
                zmq_close(ir_socket);
                zmq_close(depth_socket);
                zmq_close(registered_socket);
            }
            destroyAllWindows();
            inMenu = false;
            cout << "Noh Vimoh!" << endl;
        }
    }
}
