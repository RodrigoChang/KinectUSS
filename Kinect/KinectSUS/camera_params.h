// camera_params.h
#pragma once

class ColorCameraParams {
public:
    float fx, fy, cx, cy;
};

class Config {
public:
    float minDepth, maxDepth;
    bool enableBilateralFilter, enableEdgeAwareFilter;
};

class IrCameraParams {
public:
    float fx, fy, cx, cy, k1, k2, k3, p1, p2;
};

// Declare colorParams as extern to make it accessible from other files
extern ColorCameraParams colorParams;
extern Config depthParams;
extern IrCameraParams irParams;

// Declare the function to avoid multiple definitions
void updateParameterFile();