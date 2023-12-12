CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(KinectSUS)

# Compiler flags
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")  # Adjust compiler flags for Windows
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fpermissive -pg -O -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -g -pipe")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# ...

# Path separators (use forward slashes)
if(WIN32)
    string(REPLACE "\\" "/" source_dir "${PROJECT_SOURCE_DIR}/src")
else()
    set(source_dir "${PROJECT_SOURCE_DIR}/src")
endif()

# Explicitly list source files using forward slashes
LIST(APPEND KinectSUS_SRC
    ${source_dir}/kinect.cpp
    ${source_dir}/main.cpp
    ${source_dir}/mainMenu.cpp
    ${source_dir}/menu.cpp
    ${source_dir}/utils.cpp
    # Add more source files as needed
)

# ...


# Ruta de instalación de vcpkg
set(VCPKG_INSTALL_DIR "C:/Users/rodri/Documents/Git/vcpkg/")
include(${VCPKG_INSTALL_DIR}/scripts/buildsystems/vcpkg.cmake)

# Incluye las carpetas de Boost
include_directories(${VCPKG_INSTALL_DIR}/installed/x64-windows/include)

# Enlaza con las bibliotecas de Boost
link_directories(${VCPKG_INSTALL_DIR}/installed/x64-windows/lib)

FIND_PACKAGE(freenect2 REQUIRED)
SET(freenect2_LIBRARIES freenect2)

FIND_PACKAGE(PCL REQUIRED)

if(PCL_FOUND)
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})
endif()

# Add OpenCV package dependency for udp-image-streaming
find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind COMMAND ${VALGRIND} --tool=callgrind ./KinectSUS ARGS)
endif()

# Toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/rodri/Documents/Git/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# load in pkg-config support
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

# Libraries and Include Directories
include_directories(${OpenCV_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include ${ZeroMQ_INCLUDE_DIR})
# Ejecutable
ADD_EXECUTABLE(KinectSUS ${KinectSUS_SRC})

TARGET_LINK_LIBRARIES(KinectSUS
  ${KinectSUS_LIBRARIES} 
  ${freenect2_LIBRARIES} 
  ${ZeroMQ_LIBRARY}
  ${PCL_LIBRARIES}
  ${OpenCV_LIBS}
)
