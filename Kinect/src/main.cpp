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
zmq::socket_t read_socket(context, ZMQ_SUB);

libfreenect2::Freenect2 freenect2;
libfreenect2::Freenect2Device* dev = 0;
libfreenect2::PacketPipeline* pipeline = 0;

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
    //read_socket.connect("tcp://10.171.30.11:5558");

    read_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    cout << "Sockets listos" << endl;
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

void mensaje() {
    zmq::message_t read_message;
    read_socket.recv(&read_message);

    // Assuming the message is a string in the format "x,y"
    string coordinates = std::string(static_cast<char*>(read_message.data()), read_message.size());

    // Parse the coordinates
    size_t commaPos = coordinates.find(',');
    if (commaPos != std::string::npos) {
        int x = std::stoi(coordinates.substr(0, commaPos));
        int y = std::stoi(coordinates.substr(commaPos + 1));

        // Use x and y as needed
        std::cout << "Received coordinates: x = " << x << ", y = " << y << std::endl;
    }
}

int main(int argc, char *argv[]) {
    cerr << "Argumentos: [-norgb | -nodepth | -nostream] " << endl;
    MyFileLogger *filelogger = new MyFileLogger(getenv("LOGFILE"));
    if (filelogger->good()) libfreenect2::setGlobalLogger(filelogger);
    else delete filelogger;
    //File logger

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
            pipeline = new libfreenect2::OpenGLPacketPipeline();
            if (pipeline) dev = freenect2.openDevice(serial, pipeline);
            else dev = freenect2.openDevice(serial);      
            if (dev == 0) {
                cout << "No se pudo abrir la kinect" << endl;
                int conf = confirmacion();
                if(!conf) return 0;
                else continue;
            }   

            signal(SIGINT, sigint_handler);
            protonect_shutdown = false;
            // Initialize OpenCV window for the Kinect stream
            namedWindow("registered");
            setMouseCallback("registered", onMouseCallback);

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
            dev->start();
            cout << "No de serie: " << dev->getSerialNumber() << endl;
            cout << "Firmware de la Kinect : " << dev->getFirmwareVersion() << endl;
            libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
            libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
        
            while (!protonect_shutdown) {   //Mientras spawneen mas frames va a seguir ejecutandose, ***TO DO***  Buscar una manera de limitarlos?
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
                //registration
                registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb);
                //point cloud
                //getCloudData(registration, &undistorted);
                getCloudDataRGB(registration, &undistorted, &registered);
                //getCloudDataRGB2(registration, &undistorted, &registered);
                //visualizePointCloud();
                visualizePointCloudRGB();
                cloud_rgb->clear();
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
                //imshow("ir", irmat / 4096.0f);
                imshow("depth", depthmat / 4096.0f);
                //imshow("undistorted", depthmatUndistorted / 4096.0f);
                imshow("registered", rgbd);
                //imshow("depth2RGB", rgbd2 / 4096.0f);
                imshow("cropped", cropped);

                //auto t1 = high_resolution_clock::now();

                if (enable_stream) {
                   // auto t1 = high_resolution_clock::now();
                    send_zmq(cropped, move(rgb_socket), true, "rgb");
                    send_zmq(irmat, move(ir_socket), false, "ir");
                    send_zmq(depthmat, move(depth_socket), false, "depth");
                    send_zmq(rgbd, move(registered_socket), true, "registered");
                    //auto t2 = high_resolution_clock::now();
                    //auto ms_int = duration_cast<milliseconds>(t2 - t1);
                    //cout << ms_int.count() << "ms\n";
                }
            
                //auto t2 = high_resolution_clock::now();
                //this_thread::sleep_for(20ms);
                int key = waitKey(1);
                protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));
                listener.release(frames);

                // auto ms_int = duration_cast<milliseconds>(t2 - t1);
                // cout << ms_int.count() << "ms\n";
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
