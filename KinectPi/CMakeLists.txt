## Headers
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

#Nombre proyecto

PROJECT(KinectSUS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build types
set(CMAKE_CONFIGURATION_TYPES "Release;Debug")

# Compiler flags for debug build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wformat-security -pthread -fpermissive -pg -Og -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -g")

# Compiler flags for release build
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Ofast -fpermissive -pthread --param max-completely-peeled-insns=8 -fno-inline --param case-values-threshold=20")

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # Add any additional flags common to both release and debug builds

SET (source_dir "${PROJECT_SOURCE_DIR}/src/")
FILE (GLOB KinectSUS_SRC "${source_dir}/*.cpp")

#Dependencias

FIND_PACKAGE(freenect2 REQUIRED)
SET(freenect2_LIBRARIES freenect2)

# Add OpenCV package dependency for udp-image-streaming
find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind COMMAND ${VALGRIND} --tool=callgrind ./KinectSUS ARGS)
endif()
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(jsoncpp REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include /usr/include)
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
