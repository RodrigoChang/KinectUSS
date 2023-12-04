## Headers
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

#Nombre proyecto

PROJECT(KinectSUS)

SET (CMAKE_CONFIGURATION_TYPES "Debug")
# Compiler flags
# si esta en debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat-security -pthread -fpermissive -pg -O -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -g -pipe")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# si esta en produccion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Ofast -pthread")

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
FIND_PACKAGE(PCL REQUIRED)
FIND_PACKAGE(jsoncpp REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include /usr/include)
LIST(APPEND KinectSUS_LIBRARIES
  ${OpenCV_LIBS} ${PCL_LIBRARIES} jsoncpp
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
