#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include "utils.h"
#include <nlohmann/json.hpp>
#include <chrono>

void cloud(libfreenect2::Frame undistorted) {

    //using namespace std::chrono_literals;
    //std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
    bool value = true;
    int r = 0 , c = 0

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    while (value && r < 512 && c < 424) {
        // Extract the x, y, z coordinates for the current pixel
        float x, y, z;
        libfreenect2::Registration::getPointXYZ(undistorted, r, c, &x, &y, &z);

        // Create a point and add it to the cloud
        pcl::PointXYZ point;
        point.x = x;
        point.y = y;
        point.z = z;
        cloud->points.push_back(point);

        if (r == 512 && c == 424) {
            value = false;
        }

        c++;
        if (c >= 424) {
            c = 0;
            r++;
        }
    }

    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));

    // Add the point cloud to the viewer
    viewer->addPointCloud(cloud, "cloud");

    // Set the viewer to display the point cloud
    viewer->spin();
}