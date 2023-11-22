#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include "utils.h"
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

//libfreenect2::Frame* undistorted;

void visualizePointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));

    // Add the point cloud to the viewer
    viewer->addPointCloud(cloud, "cloud");
    std::cout << "agredado" << endl;
    // Set the viewer to display the point cloud
    viewer->spinOnce();
    std::cout << "visualizado" << endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
}