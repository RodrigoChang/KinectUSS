// Este es el main.cpp que junto a menu.cpp menu.h camera_params.h crea un menu bonito que hace config.
#include <csignal>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include "../include/utils.h"
#include "../include/log.h"
#include <thread>
#include <chrono>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

static bool started = true, enable_rgb = true, enable_depth = true, enable_stream = true, streaming = true;

static libfreenect2::Freenect2 freenect2;
libfreenect2::Freenect2Device* dev = 0;

void mainMenu();
void sigint_handler(int s) {
    streaming = false;
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
    while(started) {
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
            if (dev == 0) {
                cout << "No se pudo abrir la kinect" << endl;
                int conf = confirmacion();
                if(!conf) return 0;
                else continue;
            } 
            thread Kinect1(kinect);
            signal(SIGINT, sigint_handler);
            while(streaming)
            mainMenu();
            cout << "Abriendo Menu" << endl;
            menu(); //Inicilizamos el menu           
        }
    }
    started = false;
    cout << "Noh Vimoh!" << endl;
}
