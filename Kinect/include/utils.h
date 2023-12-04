#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include <opencv2/opencv.hpp>
#include <zmq.hpp>
#include <string>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

//extern libfreenect2::Registration* registration;
extern libfreenect2::Freenect2Device *dev;
extern libfreenect2::Freenect2Device::ColorCameraParams ColorCameraParams;
extern libfreenect2::Freenect2Device::IrCameraParams IrCameraParams;
extern libfreenect2::Freenect2Device::Config config;

//ZMQ
class zmq_stream {
private:
    zmq::context_t* context;
    zmq::socket_t* socket;
    std::string serverAddress;
    int socketType;

public:
    zmq_stream(const std::string& serverAddress, int socketType);
    ~zmq_stream();

    void encodeo_envio(const cv::Mat& frame);
    void envio_plain(const cv::Mat& frame);

private:
    std::vector<uchar> encodeo(const cv::Mat& frame);
};

//Cloud
class PointCloud {
    private:
    pcl::visualization::PCLVisualizer viewer;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_rgb;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
    std::ofstream outputFile;

public:
    PointCloud() : viewer("Visor de Point Cloud") {}

    void getPointCloud(libfreenect2::Registration* registration, 
                    libfreenect2::Frame* undistorted_frame, 
                    libfreenect2::Frame*  registered_frame = NULL, int type = 0);

    void visualizePointCloud();
    void visualizePointCloudRGB();

private:
    void getCloudData(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame);

    void getCloudDataRGB(libfreenect2::Registration* registration, 
                        libfreenect2::Frame* undistorted_frame, 
                        libfreenect2::Frame* registered_frame);

    void getCloudDataRGB2(libfreenect2::Registration* registration, 
                        libfreenect2::Frame* undistorted_frame, 
                        libfreenect2::Frame* registered_frame);

    template <typename PointType>
    void fixCloud(const typename pcl::PointCloud<PointType>::Ptr&  processedCloud);
    template <typename PointType>
    void to_csv(const typename pcl::PointCloud<PointType>::Ptr&  processedCloud);

};

//Kinect
void kinect();

//utils.cpp
void readIni();
void writeIni();
void getParams(libfreenect2::Freenect2Device *dev);
void setParams(libfreenect2::Freenect2Device *dev);
int confirmacion();
//depth menu
void menu();
