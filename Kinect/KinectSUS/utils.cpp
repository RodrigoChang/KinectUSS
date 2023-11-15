#include <fstream>
#include "camera_params.h"

void updateParameterFile() {
    std::ofstream paramFile("parameters.txt");
    paramFile << "ColorCameraParams: " << colorParams.fx << " " << colorParams.fy << " " << colorParams.cx << " " << colorParams.cy << std::endl;
    paramFile << "Config: " << depthParams.minDepth << " " << depthParams.maxDepth << " " << depthParams.enableBilateralFilter << " " << depthParams.enableEdgeAwareFilter << std::endl;
    paramFile << "IrCameraParams: " << irParams.fx << " " << irParams.fy << " " << irParams.cx << " " << irParams.cy << " " << irParams.k1 << " " << irParams.k2 << " " << irParams.k3 << " " << irParams.p1 << " " << irParams.p2 << std::endl;
    paramFile.close();
}