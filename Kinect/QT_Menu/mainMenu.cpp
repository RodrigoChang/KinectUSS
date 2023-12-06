#include "../include/utils.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/logger.h>
#include <chrono>
#include <thread>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

using namespace cv;
using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; 

// Assuming you have a class variable named `ui` representing the UI instance
Ui::KinectUSS ui;

// Color Camera Params
float Cfx = 1081, Cfy = 1081, Ccx = 959, Ccy = 539;

// IR Camera Params
float Ifx = 366, Ify = 366, Icx = 257, Icy = 204, Ik1 = 1086489, Ik2 = 264462, Ik3 = 98487, Ip1 = 0, Ip2 = 0;

// Color Camera Menu
void updateColorSliders() {
    ui.Coef_Distorsion_Slider_1->setValue(Cfx);
    ui.Coef_Distorsion_Slider_2->setValue(Cfy);
    ui.Coef_Distorsion_Slider_3->setValue(Ccx);
    // Update other color sliders similarly
}

void onColorSliderFx(int pos, void* userdata) {
    Cfx = static_cast<float>(pos);
    dev->setColorCameraParams(ColorCameraParams);
    updateColorSliders();
}

void onColorSliderFy(int pos, void* userdata) {
    Cfy = static_cast<float>(pos);
    dev->setColorCameraParams(ColorCameraParams);
    updateColorSliders();
}

// Similarly update onColorSliderCx and onColorSliderCy functions

// IR Camera Menu
void updateIrSliders() {
    ui.Slider_focal_X_Ir->setValue(Ifx);
    ui.Slider_focal_Y_Ir->setValue(Ify);
    ui.Slider_principal_X_Ir->setValue(Icx);
    ui.Slider_principal_Y_Ir->setValue(Icy);
    ui.Coef_Distorsion_Slider_1->setValue(Ik1);
    ui.Coef_Distorsion_Slider_2->setValue(Ik2);
    ui.Coef_Distorsion_Slider_3->setValue(Ik3);
    // Update other IR sliders similarly
}

void onIrSliderFx(int pos, void* userdata) {
    Ifx = static_cast<float>(pos);
    dev->setIrCameraParams(IrCameraParams);
    updateIrSliders();
}

// Similarly update onIrSliderFy, onIrSliderCx, onIrSliderCy, onIrSliderK1, onIrSliderK2, onIrSliderK3 functions

// Callback function for the button
void onCallbackButton3(int state, void* userdata) {
    // Handle button click
}

void mainMenu() {
    // Assuming you have a QMainWindow instance named `window`
    QMainWindow window;
    ui.setupUi(&window);

    // Connect sliders and button to their corresponding callback functions
    QObject::connect(ui.Coef_Distorsion_Slider_1, &QSlider::valueChanged, onColorSliderFx);
    QObject::connect(ui.Coef_Distorsion_Slider_2, &QSlider::valueChanged, onColorSliderFy);
    // Connect other color sliders similarly

    QObject::connect(ui.Slider_focal_X_Ir, &QSlider::valueChanged, onIrSliderFx);
    // Connect other IR sliders similarly

    QObject::connect(ui.Reset_Kinect, &QPushButton::clicked, onCallbackButton3);

    // Display the UI
    window.show();

    // Update UI elements with initial values
    updateColorSliders();
    updateIrSliders();

    // Run the application event loop
    QApplication::exec();
}
