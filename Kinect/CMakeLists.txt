## Headers
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

#Nombre proyecto

PROJECT(KinectSUS)

SET (CMAKE_CONFIGURATION_TYPES "Debug")
# Compiler flags
# si esta en debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat-security -pthread -O -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -g -pipe")
# si esta en produccion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Ofast -pthread")

SET (source_dir "${PROJECT_SOURCE_DIR}/src/")
FILE (GLOB KinectSUS_SRC "${source_dir}/*.cpp")
set(CMAKE_TOOLCHAIN_FILE "C:/Users/rodri/Documents/Git/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

#Dependencias
# Ruta de instalación de vcpkg
set(VCPKG_INSTALL_DIR "C:/Users/rodri/Documents/Git/vcpkg/")
include(${VCPKG_INSTALL_DIR}/scripts/buildsystems/vcpkg.cmake)
# Incluye las carpetas de Boost
include_directories(${VCPKG_INSTALL_DIR}/installed/x64-windows/include)

# Enlaza con las bibliotecas de Boost
link_directories(${VCPKG_INSTALL_DIR}/installed/x64-windows/lib)



FIND_PACKAGE(freenect2 REQUIRED)
SET(freenect2_LIBRARIES freenect2)

# Add OpenCV package dependency for udp-image-streaming
FIND_PACKAGE(OpenCV REQUIRED)
#FIND_PACKAGE(PCL REQUIRED)
FIND_PACKAGE(jsoncpp REQUIRED)
#FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem iostreams)
#target_link_libraries(KinectSUS ${Boost_LIBRARIES})
#INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/headers /usr/include)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/headers /usr/include)
#LIST(APPEND KinectSUS_LIBRARIES
#  ${OpenCV_LIBS} ${PCL_LIBRARIES} jsoncpp
#)
LIST(APPEND KinectSUS_LIBRARIES
  ${OpenCV_LIBS} jsoncpp
)
## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

#Ejecutable

ADD_EXECUTABLE(KinectSUS ${KinectSUS_SRC})

TARGET_LINK_LIBRARIES(KinectSUS
  ${KinectSUS_LIBRARIES} 
  ${freenect2_LIBRARIES} 
  ${ZeroMQ_LIBRARY}
)
