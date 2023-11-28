#include <iostream>
#include <fstream>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/point_cloud_color_handlers.h>
#include <pcl/common/common_headers.h>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/registration.h>
#include "../include/utils.h"
#include <chrono>
#include <thread>

pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_rgb(new pcl::PointCloud<pcl::PointXYZRGB>);
pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));
std::ofstream outputFile;

void getCloudData(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame) {
    int r = 0, c = 0;

        while (r < 512 && c < 424) {
            static float x, y, z;
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
    for (size_t i = 0; i < cloud->points.size(); ++i) {
        cloud->points[i].z = -cloud->points[i].z;
    }
}   

void getCloudDataRGB(libfreenect2::Registration* registration, 
                    libfreenect2::Frame* undistorted_frame, 
                    libfreenect2::Frame* registered_frame) {

    cloud_rgb->header.frame_id = "world";
    cloud_rgb->reserve(512 * 424);
    for(int r = 0; r < 424; ++r) {
        for(int c = 0; c < 512; ++c) {
            pcl::PointXYZRGB pt;
            registration->getPointXYZ(undistorted_frame, r, c, pt.x, pt.y, pt.z);
            pt.rgb = *reinterpret_cast<float*>(registered_frame->data + 4 * (r * 512 + c));
            cloud_rgb->push_back(pt);
        }
    }

    for (size_t i = 0; i < cloud_rgb->points.size(); ++i) {
        cloud_rgb->points[i].y = -cloud_rgb->points[i].y;
    }
    for (size_t i = 0; i < cloud_rgb->points.size(); ++i) {
        cloud_rgb->points[i].z = -cloud_rgb->points[i].z;
    }
}  

void visualizePointCloud() {
   // Calculate min and max Z values
    float min_z = std::numeric_limits<float>::infinity();
    float max_z = -std::numeric_limits<float>::infinity();

    for (size_t i = 0; i < cloud->points.size(); ++i) {
        float z = cloud->points[i].z;
        if (z < min_z) min_z = z;
        if (z > max_z) max_z = z;
    }
    // Open the output CSV file
    outputFile.open("point_cloud.csv");

    // Save each point to the CSV file 
    for (size_t i = 0; i < cloud->points.size(); ++i) {
        outputFile << cloud->points[i].x << "," << cloud->points[i].y << "," << cloud->points[i].z << "\n";
    }
    
    pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> color_handler(cloud, "z");
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> color_handler_custom(cloud, 0, 255, 0); // Default color if colormap fails
    viewer->addPointCloud<pcl::PointXYZ>(cloud, color_handler, "cloud");
    // Set point size
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");

    // Set the viewer to display the point cloud
    viewer->spinOnce();

    viewer->removePointCloud("cloud");
    cloud->clear();
}
void visualizePointCloudRGB() {
    // Open the output CSV file
    outputFile.open("point_cloud.csv");

    // Save each point to the CSV file 
    for (size_t i = 0; i < cloud_rgb->points.size(); ++i) {
        outputFile << cloud_rgb->points[i].x << "," << cloud_rgb->points[i].y << "," << cloud_rgb->points[i].z << "\n";
    }
    
    //pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGB> color_handler_c(cloud_rgb, 0, 255, 0); // Default color if colormap fails
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> color_handler(cloud_rgb);
    viewer->addPointCloud<pcl::PointXYZRGB>(cloud_rgb, color_handler, "cloud");
    // Set point size
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");

    // Set the viewer to display the point cloud
    viewer->spinOnce();

    viewer->removePointCloud("cloud");
    cloud_rgb->clear();
}