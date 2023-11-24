#ifndef UTILS_H
#define UTILS_H

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include <opencv2/opencv.hpp>
#include <zmq.hpp>
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>


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

void send_zmq(cv::Mat& frame, zmq::socket_t&& socket, bool encodeado, std::string tipo);

//void generatePointCloud(libfreenect2::Frame undistorted, libfreenect2::Registration* registration, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);
void visualizePointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::visualization::PCLVisualizer::Ptr viewer);

#endif // UTILS_H