#include "../include/utils.h"
//#include "menu.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include <chrono>
#include <thread>

using namespace cv;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; 

int minD = 500, maxD = 4500;
bool bFilter, eFilter = true;
libfreenect2::Freenect2Device::Config config;
libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
//libfreenect2::Registration* registration;
//libfreenect2::Freenect2Device* dev;

// depth menu

void onMinDepthSlider(int pos, void* userdata) {
    if (pos >= maxD) pos = 0;
    config.MinDepth = static_cast<float>(minD) / 1000;
    dev->setConfiguration(config);
}

void onMaxDepthSlider(int pos, void* userdata) {
    if (maxD <= minD) maxD = minD + 1;
    config.MaxDepth = static_cast<float>(maxD) / 1000;
    dev->setConfiguration(config);
}

void onCallbackButton1(int state, void* userdata) {
    if (state == 0) 
        config.EnableBilateralFilter = false;
    else config.EnableBilateralFilter = true;
    dev->setConfiguration(config);
}

void onCallbackButton2(int state, void* userdata) {
    if (state == 0) 
        config.EnableEdgeAwareFilter = false;
    else config.EnableEdgeAwareFilter = true;
    dev->setConfiguration(config);
}

// reiniciar registration

void menu() {

    namedWindow("Depth Config", WINDOW_NORMAL);
    Mat depthmenu(400, 800, CV_8UC3, Scalar(255, 255, 255));
    // depth
    createTrackbar("MinDepth", "Depth Config", &minD, 8500, onMinDepthSlider);
    createTrackbar("MaxDepth", "Depth Config", &maxD, 8500, onMaxDepthSlider);
    createButton("Bilateral Filtering", onCallbackButton1, NULL, QT_CHECKBOX, 1);
    createButton("Edge Aware Filtering", onCallbackButton2, NULL, QT_CHECKBOX, 1);

    imshow("Depth Config", depthmenu);
}
