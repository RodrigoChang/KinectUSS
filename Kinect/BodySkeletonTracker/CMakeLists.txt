## Headers
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12.1)

SET(PROJECT_VER_MAJOR 0)
SET(PROJECT_VER_MINOR 2)
SET(PROJECT_VER_PATCH 0)
SET(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
SET(PROJECT_APIVER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}")


file(GLOB_RECURSE sources src/*.cpp)
INCLUDE_DIRECTORIES(include)
#Nombre proyecto

PROJECT(BodySkeletonTracker)

# Variables
SET(MY_DIR ${BodySkeletonTracker_SOURCE_DIR})
INCLUDE_DIRECTORIES("${MY_DIR}/include" "${MY_DIR}/OpenNI-Linux-x64-2.2/Include")
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

#INCLUDE_DIRECTORIES(${OpenNI2_DIR}) #Deberia funcionar pero no po

# Add OpenCV package dependency for udp-image-streaming
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LIST(APPEND BodySkeletonTracker_LIBRARIES
  ${OpenCV_LIBS}
)
# OpenCV requires exception.
IF(UNIX AND NOT APPLE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
ENDIF()

#Ejecutable

ADD_EXECUTABLE(BodySkeletonTracker ${sources})

TARGET_LINK_LIBRARIES(BodySkeletonTracker
  ${BodySkeletonTracker_LIBRARIES} ${freenect2_LIBRARIES}
)
