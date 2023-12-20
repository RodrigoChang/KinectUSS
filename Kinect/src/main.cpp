// Este es el main.cpp que junto a menu.cpp menu.h camera_params.h crea un menu bonito que hace config.
#include <csignal>
#include <iostream>
#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include "../include/utils.h"
#include "../include/log.h"
#include <thread>
#include <chrono>

using namespace std;

bool enable_rgb, enable_depth, enable_stream, enable_cloud, enable_z_stream, onStreaming = true, protonect_shutdown = false;
static bool pRunning = true;
int tipo_cloud; 
string ip;
chrono::milliseconds frametime(33);
libfreenect2::Freenect2 freenect2;
libfreenect2::Freenect2Device* dev = 0;

void mainMenu();
void sigint_handler(int s) {
    protonect_shutdown = true;
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

int main() {
    //File logger
    MyFileLogger *filelogger = new MyFileLogger(getenv("LOGFILE"));
    if (filelogger->good()) libfreenect2::setGlobalLogger(filelogger);
    else delete filelogger;

    //conf para no hacerme la vida tan dificil
    string filename = "../config.conf";
    map<string, string> config = readConfigFile(filename);

    for (const auto& pair : config) {
        cout << pair.first << " = " << pair.second << endl;
        if (pair.first == "ip_mp") ip = pair.second;
        if (pair.first == "rgb") {
            if (pair.second == "True") enable_rgb = true;
            else enable_rgb = false;
        }
        if (pair.first == "depth") {
            if (pair.second == "True") enable_depth = true;
            else enable_depth = false;
        }
        if (pair.first == "stream") {
            if (pair.second == "True") enable_stream = true;
            else enable_stream = false;
        }
         if (pair.first == "z_stream") {
            if (pair.second == "True") enable_z_stream = true;
            else enable_z_stream = false;
        }
        if (pair.first == "cloud") {
            if (pair.second == "gray") {
                tipo_cloud == PointCloud::GRAY;
                enable_cloud = true;
            }
            else if (pair.second == "rgb") {
                tipo_cloud == PointCloud::RGB;
                enable_cloud = true; 
            }
            else if (pair.second == "rgb2") {
                tipo_cloud == PointCloud::RGB2;
                enable_cloud = true;
            }
            else enable_cloud = false;
        }
        if (pair.first == "frametime") {
            long long number = stoll(pair.second);
            chrono::milliseconds frametime(number);
        }
    } 
    if (!enable_rgb && !enable_depth) {
        cerr << "toncs que stremeamos?" << endl;
        return -1;
    }
    cout << "Inicializando sistema" << endl;
    while(pRunning) {
        bool found = kinectSearch();
        if (!found) {
            int conf = confirmacion();
            if(!conf) return 0;
            else continue;
        } else {
            string serial = freenect2.getDefaultDeviceSerialNumber();
            thread Kinect1(kinect, serial);
            signal(SIGINT, sigint_handler);
            this_thread::sleep_for(chrono::milliseconds(5000));
            //thread mainM_thread(mainMenu);
            while(onStreaming) {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            //Kinect1.join();
            //mainM_thread.join();
           // menu_thread.join();
            pRunning = false;
        }
    }
    cout << "Noh Vimoh!" << endl;
    return 0;
}
