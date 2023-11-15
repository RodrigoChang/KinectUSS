#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "camera_params.h"
#include "menu.h"

using namespace cv;
using namespace std;
using namespace libfreenect2;

void updateParameterFile() {
    ofstream paramFile("parameters.txt");
    paramFile << "ColorCameraParams: " << colorParams.fx << " " << colorParams.fy << " " << colorParams.cx << " " << colorParams.cy << endl;
    paramFile << "Config: " << depthParams.minDepth << " " << depthParams.maxDepth << " " << depthParams.enableBilateralFilter << " " << depthParams.enableEdgeAwareFilter << endl;
    paramFile << "IrCameraParams: " << irParams.fx << " " << irParams.fy << " " << irParams.cx << " " << irParams.cy << " " << irParams.k1 << " " << irParams.k2 << " " << irParams.k3 << " " << irParams.p1 << " " << irParams.p2 << endl;
    paramFile.close();
}

void onColorSlider(int value, void* userdata) {
    ColorCameraParams* params = static_cast<ColorCameraParams*>(userdata);
    updateParameterFile();
    // ... rest of the code
}

void onDepthSlider(int value, void* userdata) {
    Config* params = static_cast<Config*>(userdata);
    updateParameterFile();
    // ... rest of the code
}

void onIRSlider(int value, void* userdata) {
    IrCameraParams* params = static_cast<IrCameraParams*>(userdata);
    updateParameterFile();
    // ... rest of the code
}

void menu() {
    // Load previously saved parameters
    ifstream paramFile("parameters.txt");
    if (paramFile.is_open()) {
        string line;
        while (getline(paramFile, line)) {
            istringstream iss(line);
            string paramType;
            iss >> paramType;
            if (paramType == "ColorCameraParams:") {
                iss >> colorParams.fx >> colorParams.fy >> colorParams.cx >> colorParams.cy;
            } else if (paramType == "Config:") {
                iss >> depthParams.minDepth >> depthParams.maxDepth >> depthParams.enableBilateralFilter >> depthParams.enableEdgeAwareFilter;
            } else if (paramType == "IrCameraParams:") {
                iss >> irParams.fx >> irParams.fy >> irParams.cx >> irParams.cy >> irParams.k1 >> irParams.k2 >> irParams.k3 >> irParams.p1 >> irParams.p2;
            }
        }
    }
    paramFile.close();

    Mat menus(400, 800, CV_8UC3, Scalar(255, 255, 255));

    createTrackbar("fx", "ColorCameraParams", reinterpret_cast<int*>(&colorParams.fx), 100, onColorSlider, &colorParams);
    createTrackbar("fy", "ColorCameraParams", reinterpret_cast<int*>(&colorParams.fy), 100, onColorSlider, &colorParams);
    createTrackbar("cx", "ColorCameraParams", reinterpret_cast<int*>(&colorParams.cx), 100, onColorSlider, &colorParams);
    createTrackbar("cy", "ColorCameraParams", reinterpret_cast<int*>(&colorParams.cy), 100, onColorSlider, &colorParams);


    createTrackbar("Min Depth", "DepthConfig", reinterpret_cast<int*>(&depthParams.minDepth), 1000, onDepthSlider, &depthParams);
    createTrackbar("Max Depth", "DepthConfig", reinterpret_cast<int*>(&depthParams.maxDepth), 5000, onDepthSlider, &depthParams);
    createTrackbar("Enable Bilateral Filter", "DepthConfig", reinterpret_cast<int*>(&depthParams.enableBilateralFilter), 1, onDepthSlider, &depthParams);
    createTrackbar("Enable Edge Aware Filter", "DepthConfig", reinterpret_cast<int*>(&depthParams.enableEdgeAwareFilter), 1, onDepthSlider, &depthParams);

    createTrackbar("fx", "IRCameraParams", reinterpret_cast<int*>(&irParams.fx), 100, onIRSlider, &irParams);
    createTrackbar("fy", "IRCameraParams", reinterpret_cast<int*>(&irParams.fy), 100, onIRSlider, &irParams);
    createTrackbar("cx", "IRCameraParams", reinterpret_cast<int*>(&irParams.cx), 100, onIRSlider, &irParams);
    createTrackbar("cy", "IRCameraParams", reinterpret_cast<int*>(&irParams.cy), 100, onIRSlider, &irParams);
    createTrackbar("k1", "IRCameraParams", reinterpret_cast<int*>(&irParams.k1), 100, onIRSlider, &irParams);
    createTrackbar("k2", "IRCameraParams", reinterpret_cast<int*>(&irParams.k2), 100, onIRSlider, &irParams);
    createTrackbar("k3", "IRCameraParams", reinterpret_cast<int*>(&irParams.k3), 100, onIRSlider, &irParams);
    createTrackbar("p1", "IRCameraParams", reinterpret_cast<int*>(&irParams.p1), 100, onIRSlider, &irParams);
    createTrackbar("p2", "IRCameraParams", reinterpret_cast<int*>(&irParams.p2), 100, onIRSlider, &irParams);

    imshow("Menu", menus);
    //waitKey(0);

  //  return 0;
}
