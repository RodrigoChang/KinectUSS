## Headers
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12.1)

SET(PROJECT_VER_MAJOR 0)
SET(PROJECT_VER_MINOR 2)
SET(PROJECT_VER_PATCH 0)
SET(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
SET(PROJECT_APIVER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}")

#Nombre proyecto

PROJECT(KinectSUS)

# Variables

IF(NOT DEFINED CMAKE_BUILD_TYPE)
  # No effect for multi-configuration generators (e.g. for Visual Studio)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose: RelWithDebInfo Release Debug MinSizeRel None")
ENDIF()

IF(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
ENDIF()

#Dependencias

FIND_PACKAGE(freenect2 REQUIRED)
SET(freenect2_LIBRARIES freenect2)


#INCLUDE_DIRECTORIES(${freenect2_DIR}) #Deberia funcionar pero no po
#SET (KinectSUS_DIR main.cpp menu.cpp camera_params.h menu.h)
SET (KinectSUS_DIR kinect.cpp)

# Add OpenCV package dependency for udp-image-streaming
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LIST(APPEND KinectSUS_LIBRARIES
  ${OpenCV_LIBS}
)
# OpenCV requires exception.
IF(UNIX AND NOT APPLE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
ENDIF()

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from above to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

#Ejecutable

ADD_EXECUTABLE(KinectSUS ${KinectSUS_DIR})

TARGET_LINK_LIBRARIES(KinectSUS
  ${KinectSUS_LIBRARIES} ${freenect2_LIBRARIES} ${ZeroMQ_LIBRARY}
)
