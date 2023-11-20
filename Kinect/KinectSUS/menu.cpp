#include "utils.h"
#include "menu.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include <chrono>
#include <thread>

using namespace cv;
using namespace std;
using namespace libfreenect2;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; 

int minD = 500;
int maxD = 4500;
bool bFilter, eFilter = true;
libfreenect2::Freenect2Device::Config config;
libfreenect2::Freenect2Device* dev = 0;

void onMinDepthSlider(int pos, void* userdata) {
    if (pos >= maxD) {
        pos = 0;
    }
    config.MinDepth = static_cast<float>(minD) / 1000;
    cout << "Min value: " << static_cast<float>(pos) / 1000 << endl;
    dev->setConfiguration(config);
}

void onMaxDepthSlider(int pos, void* userdata) {
    if (maxD <= minD) {
        maxD = minD + 1;
    }
    config.MaxDepth = static_cast<float>(maxD) / 1000;
    dev->setConfiguration(config);
}


void onCallbackButton1(int state, void* userdata)
{
    if (state == 0) {
        config.EnableBilateralFilter = false;
    }
    else if (state == 1) {
        config.EnableBilateralFilter = true;
    }
}

void onCallbackButton2(int state, void* userdata)
{
    if (state == 0) {
        config.EnableEdgeAwareFilter = false;
    }
    else if (state == 1) {
        config.EnableEdgeAwareFilter = true;
    }
}

void menu(libfreenect2::Freenect2Device* device) {
    dev = device;
    namedWindow("Menu", WINDOW_NORMAL);
    Mat menus(400, 800, CV_8UC3, Scalar(255, 255, 255));

    createTrackbar("MinDepth", "Menu", &minD, 8500, onMinDepthSlider);
    createTrackbar("MaxDepth", "Menu", &maxD, 8500, onMaxDepthSlider);
    createButton("Bilateral Filtering", onCallbackButton1, NULL, QT_CHECKBOX, 1);
    createButton("Edge Aware Filtering", onCallbackButton2, NULL, QT_CHECKBOX, 1);
    //createTrackbar("fx", "Menu", static_cast<int>(Colorparams.fx*100), 850000, onColorSlider);
    //createTrackbar("fy", "Menu", static_cast<int>(Colorparams.fy*100), 850000, onColorSlider);
    //createTrackbar("cx", "Menu", static_cast<int>(Colorparams.cx*10), 10000, onColorSlider);
    //createTrackbar("cy", "Menu", static_cast<int>(Colorparams.cy*10), 10000, onColorSlider);

   // createTrackbar("fx", "Menu", reinerpret_cast<int*>(&irParams.fx), 100, onIRSlider, &irParams);
   // createTrackbar("fy", "Menu", reinterpret_cast<int*>(&irParams.fy), 100, onIRSlider, &irParams);
   // createTrackbar("cx", "Menu", reinterpret_cast<int*>(&irParams.cx), 100, onIRSlider, &irParams);
   // createTrackbar("cy", "Menu", reinterpret_cast<int*>(&irParams.cy), 100, onIRSlider, &irParams);
   // createTrackbar("k1", "Menu", reinterpret_cast<int*>(&irParams.k1), 100, onIRSlider, &irParams);
   // createTrackbar("k2", "Menu", reinterpret_cast<int*>(&irParams.k2), 100, onIRSlider, &irParams);
   // createTrackbar("k3", "Menu", reinterpret_cast<int*>(&irParams.k3), 100, onIRSlider, &irParams);
   // createTrackbar("p1", "Menu", reinterpret_cast<int*>(&irParams.p1), 100, onIRSlider, &irParams);
   // createTrackbar("p2", "Menu", reinterpret_cast<int*>(&irParams.p2), 100, onIRSlider, &irParams);t

    imshow("Menu", menus);
}
