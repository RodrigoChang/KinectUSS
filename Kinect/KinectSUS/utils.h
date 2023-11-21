#ifndef UTILS_H
#define UTILS_H

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include <opencv2/opencv.hpp>

extern libfreenect2::Registration* registration;
extern libfreenect2::Freenect2Device* dev;
extern libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
extern libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
extern libfreenect2::Freenect2Device::Config config;

void readIni();

void writeIni();

// get set
void getParams(libfreenect2::Freenect2Device* dev);
void setParams(libfreenect2::Freenect2Device* dev);

void rgbdSocket(cv::Mat rgbd);

#endif // UTILS_H