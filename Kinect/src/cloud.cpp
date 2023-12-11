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
#include <thread>

/* 
Clase PointCloud
Esta clase se encarga de crear un viewer, craftear el cloud y visualizarlo
la idea es que como cloud_rgb toma 22ms, crear una clase hace que no instanciemos el objeto
si no lo necesitamos, ahorrando mas rendimiento que memoria.
xd
*/
//PointCloud::PointCloud() : viewer("Visor de Point Cloud") {}

void PointCloud::getPointCloud(libfreenect2::Registration* registration, 
                libfreenect2::Frame* undistorted_frame,
                cloud_type type, 
                libfreenect2::Frame*  registered_frame = nullptr) {
    std::cout << "Partimos" << endl;                 
    if (registered_frame == nullptr || type == GRAY) {
        std::cout << "XYZ" << endl;
        getCloudData(registration, undistorted_frame);
        fixCloud<pcl::PointXYZ>(cloud);
        visualizePointCloud();                    
    }
    else {
        if (type == RGB2) getCloudDataRGB2(registration, undistorted_frame, registered_frame);
        else getCloudDataRGB(registration, undistorted_frame, registered_frame);
        fixCloud<pcl::PointXYZRGB>(cloud_rgb);
        visualizePointCloudRGB();
    }
}

void PointCloud::visualizePointCloud() {
    // Checkeo basico
    if (!cloud->empty()) {
        to_csv<pcl::PointXYZ>(cloud);
        // Calculamos los valores minimos y maximos de z ***TO DO*** Arreglarlo
        float min_z = std::numeric_limits<float>::infinity();
        float max_z = -std::numeric_limits<float>::infinity();

        for (size_t i = 0; i < cloud->points.size(); ++i) {
            float z = cloud->points[i].z;
            if (z < min_z) min_z = z;
            if (z > max_z) max_z = z;
        }

        // Visualizacion
        pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> color_handler(cloud, "z");
        viewer.addPointCloud<pcl::PointXYZ>(cloud, color_handler, "cloud");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
        viewer.spinOnce();
        viewer.removePointCloud("cloud");

        cloud->clear();
    }
}

void PointCloud::visualizePointCloudRGB() {
    // Checkeo basico
    if (!cloud_rgb->empty()) {
        to_csv<pcl::PointXYZRGB>(cloud_rgb);
        // Visualizacion
        pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> color_handler(cloud_rgb);
        viewer.addPointCloud<pcl::PointXYZRGB>(cloud_rgb, color_handler, "cloud");
        viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
        viewer.spinOnce();
        viewer.removePointCloud("cloud");

        cloud_rgb->clear();
    }
}

void PointCloud::getCloudData(libfreenect2::Registration* registration, libfreenect2::Frame* undistorted_frame) {
    for(int r = 0; r < 424; ++r) {
        for(int c = 0; c < 512; ++c) {
            pcl::PointXYZ pt; // Creamos el point cloud
            registration->getPointXYZ(undistorted_frame, r, c, pt.x, pt.y, pt.z); // usando el mismo registration por tema de corrupcion de memoria
            cloud->points.push_back(pt);                                          // ***TO DO*** refrescarlo con los nuevos parametros
        }
    }
}  

void PointCloud::getCloudDataRGB(libfreenect2::Registration* registration, 
                    libfreenect2::Frame* undistorted_frame, 
                    libfreenect2::Frame* registered_frame) {

    cloud_rgb->header.frame_id = "RGB";
    cloud_rgb->reserve(512 * 424);
    for(int r = 0; r < 424; ++r) {
        for(int c = 0; c < 512; ++c) {
            pcl::PointXYZRGB pt;
            registration->getPointXYZ(undistorted_frame, r, c, pt.x, pt.y, pt.z);
            pt.rgb = *reinterpret_cast<float*>(registered_frame->data + 4 * (r * 512 + c));
            cloud_rgb->push_back(pt);
        }
    }
}

void PointCloud::getCloudDataRGB2(libfreenect2::Registration* registration, 
                    libfreenect2::Frame* undistorted_frame, 
                    libfreenect2::Frame* registered_frame) {

    cloud_rgb->header.frame_id = "RGB";
    cloud_rgb->reserve(512 * 424);
    for(int r = 0; r < 424; ++r) {
        for(int c = 0; c < 512; ++c) {
            pcl::PointXYZRGB pt;
            registration->getPointXYZRGB(undistorted_frame, registered_frame, r, c, pt.x, pt.y, pt.z, pt.rgb);
            cloud_rgb->push_back(pt);
        }
    }   
} 

template <typename PointType>
void PointCloud::fixCloud(const typename pcl::PointCloud<PointType>::Ptr&  processedCloud) {
    for (size_t i = 0; i < processedCloud->points.size(); ++i) {
        processedCloud->points[i].y = -processedCloud->points[i].y;
    }
    for (size_t i = 0; i < processedCloud->points.size(); ++i) {
        processedCloud->points[i].z = -processedCloud->points[i].z;
    }
} 

template <typename PointType>
void PointCloud::to_csv(const typename pcl::PointCloud<PointType>::Ptr&  processedCloud) {
    // Abriendo un csv
    std::ofstream outputFile("point_cloud.csv");

    // Guardamos cada punto
    for (size_t i = 0; i < processedCloud->points.size(); ++i) {
        outputFile << processedCloud->points[i].x << "," << processedCloud->points[i].y << "," << processedCloud->points[i].z << "\n";
    }
}