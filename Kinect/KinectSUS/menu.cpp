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

int minD = 500, maxD = 4500;
int Cfx = 1081, Cfy = 1081, Ccx = 959, Ccy = 539;
int Ifx = 366, Ify = 366, Icx = 257, Icy = 204, Ik1 = 1086489, Ik2 = 264462, Ik3 = 98487, Ip1 = 0, Ip2 = 0;
bool bFilter, eFilter = true;
libfreenect2::Freenect2Device::Config config;
libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
libfreenect2::Freenect2Device* dev = 0;

//unsigned int cSetting = dev->getColorSetting;
//float ex = dev->getColorSettingFloat;
//cout << "Color setting: " << cSetting << endl;
//cout << "Color setting float: " << ex << endl;
//libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;

// depth menu

void onMinDepthSlider(int pos, void* userdata) {
    if (pos >= maxD) {
        pos = 0;
    }
    config.MinDepth = static_cast<float>(minD) / 1000;
    dev->setConfiguration(config);
}

void onMaxDepthSlider(int pos, void* userdata) {
    if (maxD <= minD) {
        maxD = minD + 1;
    }
    config.MaxDepth = static_cast<float>(maxD) / 1000;
    dev->setConfiguration(config);
}

void onCallbackButton1(int state, void* userdata) {
    if (state == 0) {
        config.EnableBilateralFilter = false;
    }
    else if (state == 1) {
        config.EnableBilateralFilter = true;
    }
    dev->setConfiguration(config);
}

void onCallbackButton2(int state, void* userdata) {
    if (state == 0) {
        config.EnableEdgeAwareFilter = false;
    }
    else if (state == 1) {
        config.EnableEdgeAwareFilter = true;
    }
    dev->setConfiguration(config);
}

// Color Camera Menu

void onColorSliderFx(int pos, void* userdata) {
    ColorCameraParams.fx = static_cast<float>(Cfx);
    dev->setColorCameraParams(ColorCameraParams);
}

void onColorSliderFy(int pos, void* userdata) {
    ColorCameraParams.fy = static_cast<float>(Cfy);
    dev->setColorCameraParams(ColorCameraParams);
}

void onColorSliderCx(int pos, void* userdata) {
    ColorCameraParams.cx = static_cast<float>(Ccx);
    dev->setColorCameraParams(ColorCameraParams);
}

void onColorSliderCy(int pos, void* userdata) {
    ColorCameraParams.cy = static_cast<float>(Ccy);
    dev->setColorCameraParams(ColorCameraParams);
}

// Ir camera menu

void onIrSliderFx(int pos, void* userdata) {
    IrCameraParams.fx = static_cast<float>(pos);
    dev->setIrCameraParams(IrCameraParams);
}

void onIrSliderFy(int pos, void* userdata) {
    IrCameraParams.fy = static_cast<float>(pos);
    dev->setIrCameraParams(IrCameraParams);
}

void onIrSliderCx(int pos, void* userdata) {
    IrCameraParams.cx = static_cast<float>(pos);
    dev->setIrCameraParams(IrCameraParams);
}

void onIrSliderCy(int pos, void* userdata) {
    IrCameraParams.cy = static_cast<float>(pos);
    dev->setIrCameraParams(IrCameraParams);
}  

void onIrSliderK1(int pos, void* userdata) {
    if (pos < 1000000) {
        IrCameraParams.k1 = static_cast<float>(pos) / -1000000;
        cout << "Negativo" << endl;
    }
    else {
        IrCameraParams.k1 = static_cast<float>(pos) / 1000000;
        cout << "Positivo" << endl;
    }
    dev->setIrCameraParams(IrCameraParams);
    cout << "Paso" << endl;
}

void onIrSliderK2(int pos, void* userdata) {
    if (pos < 1000000) {
        IrCameraParams.k1 = static_cast<float>(pos) / -1000000;
    }
    else {
        IrCameraParams.k1 = static_cast<float>(pos) / 1000000;
    }
    dev->setIrCameraParams(IrCameraParams);
}

void onIrSliderK3(int pos, void* userdata) {
    if (pos < 1000000) {
        IrCameraParams.k1 = static_cast<float>(pos) / -1000000;
    }
    else {
        IrCameraParams.k1 = static_cast<float>(pos) / 1000000;
    }
    dev->setIrCameraParams(IrCameraParams);
}  

void menu(libfreenect2::Freenect2Device* device) {

    dev = device;
    namedWindow("Menu", WINDOW_NORMAL);
    Mat menus(400, 800, CV_8UC3, Scalar(255, 255, 255));
    // depth
    createTrackbar("MinDepth", "Menu", &minD, 8500, onMinDepthSlider);
    createTrackbar("MaxDepth", "Menu", &maxD, 8500, onMaxDepthSlider);
    createButton("Bilateral Filtering", onCallbackButton1, NULL, QT_CHECKBOX, 1);
    createButton("Edge Aware Filtering", onCallbackButton2, NULL, QT_CHECKBOX, 1);
    // Color
    createTrackbar("Punto Focal X Color Camara", "Menu", &Cfx, 1920, onColorSliderFx);
    createTrackbar("Punto Focal Y Color Camara", "Menu", &Cfy, 1080, onColorSliderFy);
    createTrackbar("Punto principal X Color Camara", "Menu", &Ccx, 1920, onColorSliderCx);
    createTrackbar("Punto principal Y Color Camara", "Menu", &Ccy, 1080, onColorSliderCy);
    // Ir
   // createTrackbar("Punto Focal X Ir Camara", "Menu", &Ifx, 512, onIrSliderFx);
   // createTrackbar("Punto Focal Y Ir Camara", "Menu", &Ifx, 424, onIrSliderFy);
   // createTrackbar("Punto principal X Ir Camara", "Menu", &Ifx, 512, onIrSliderCx);
   // createTrackbar("Punto principal Y Ir Camara", "Menu", &Ifx, 424, onIrSliderCy);
  //  createTrackbar("Radial distortion coefficient, 1", "Menu", &Ik1, 2000000, onIrSliderK1);
  //  createTrackbar("Radial distortion coefficient, 2", "Menu", &Ik2, 2000000, onIrSliderK2);
  // createTrackbar("Radial distortion coefficient, 1", "Menu", &Ik3, 2000000, onIrSliderK3);
   //createTrackbar("p1", "Menu", reinterpret_cast<int*>(&irParams.p1), 100, onIRSlider, &irParams);
   //createTrackbar("p2", "Menu", reinterpret_cast<int*>(&irParams.p2), 100, onIRSlider, &irParams);t

    imshow("Menu", menus);
}
