#ifndef UTILS_H
#define UTILS_H

#include <libfreenect2/libfreenect2.hpp>

extern libfreenect2::Freenect2Device* dev;
extern libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
extern libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;

void readIni();

void writeIni();

// get set
void getParams(libfreenect2::Freenect2Device* dev);
void setParams(libfreenect2::Freenect2Device* dev);

#endif // UTILS_H