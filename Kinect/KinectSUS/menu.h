#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/logger.h>
#include "camera_params.h"

using namespace cv;
using namespace std;
using namespace libfreenect2;

// Function declarations
void updateParameterFile();
void onColorSlider(int value, void* userdata);
void onDepthSlider(int value, void* userdata);
void onIRSlider(int value, void* userdata);

void menu();