#include <iostream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include "utils.h"
#include <chrono>
#include <thread>

pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));

void getCloudData(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame) {
    int r = 0, c = 0;

        while (r < 512 && c < 424) {
            float x, y, z;
            registration->getPointXYZ(undistorted_frame, r, c, x, y, z); // usando el mismo registration por tema de corrupcion de memoria ***TO DO*** refrescarlo con los nuevos parametros
            pcl::PointXYZ points; // Creamos el point cloud
            points.x = x;
            points.y = y;
            points.z = z;
            cloud->points.push_back(points);
            r++;
            if (r >= 512) {
                r = 0;
                c++;
            }
        }

    for (size_t i = 0; i < cloud->points.size(); ++i) {
        cloud->points[i].y = -cloud->points[i].y;
    }
}

void visualizePointCloud() {
    // Set visualization properties
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0); // Green points
    viewer->addPointCloud<pcl::PointXYZ>(cloud, single_color, "cloud");
    // Set point size
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
    // Set the viewer to display the point cloud
    viewer->spinOnce();
    viewer->removePointCloud("cloud");
    cloud->clear();
}