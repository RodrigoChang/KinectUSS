#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include <opencv2/opencv.hpp>
#include <C:/Users/rodri/Documents/Git/vcpkg/packages/zeromq_x64-windows/include/zmq.hpp>
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>


//extern libfreenect2::Registration* registration;
extern libfreenect2::Freenect2Device *dev;
extern libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
extern libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
extern libfreenect2::Freenect2Device::Config config;
extern pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
extern pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_rgb;
extern pcl::visualization::PCLVisualizer::Ptr viewer;

//utils.cpp
void readIni();
void writeIni();
void getParams(libfreenect2::Freenect2Device *dev);
void setParams(libfreenect2::Freenect2Device *dev);
void send_zmq(cv::Mat& frame, zmq::socket_t&& socket, bool encodeado, std::string tipo);
int confirmacion();
//depth menu
void menu();
//cloud
void getCloudData(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame);
void getCloudDataRGB(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame, libfreenect2::Frame* registered_frame);
void getCloudDataRGB2(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame, libfreenect2::Frame* registered_frame);
void visualizePointCloud();
void visualizePointCloudRGB();
