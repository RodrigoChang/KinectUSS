#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include "utils.h"
#include <chrono>
#include <thread>

void visualizePointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::visualization::PCLVisualizer::Ptr viewer) {
        // Flip the point cloud data
    for (size_t i = 0; i < cloud->points.size(); ++i) {
        cloud->points[i].y = -cloud->points[i].y;
    }

    // Set visualization properties
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0); // Green points
    viewer->addPointCloud<pcl::PointXYZ>(cloud, single_color, "cloud");

    // Set point size
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");

    // Set the viewer to display the point cloud
    viewer->spinOnce();
    viewer->removePointCloud("cloud");

}