# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build"

# Include any dependencies generated for this target.
include CMakeFiles/captura_camara.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/captura_camara.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/captura_camara.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/captura_camara.dir/flags.make

CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o: CMakeFiles/captura_camara.dir/flags.make
CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o: /home/fabian/Documents/Python\ codes/kinect/kinectcpp/KinectTacking/Kinectsus-C++.cpp
CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o: CMakeFiles/captura_camara.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o -MF CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o.d -o CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o -c "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/Kinectsus-C++.cpp"

CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/Kinectsus-C++.cpp" > CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.i

CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/Kinectsus-C++.cpp" -o CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.s

# Object files for target captura_camara
captura_camara_OBJECTS = \
"CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o"

# External object files for target captura_camara
captura_camara_EXTERNAL_OBJECTS =

captura_camara: CMakeFiles/captura_camara.dir/Kinectsus-C++.cpp.o
captura_camara: CMakeFiles/captura_camara.dir/build.make
captura_camara: /usr/local/lib/libopencv_gapi.so.4.8.0
captura_camara: /usr/local/lib/libopencv_highgui.so.4.8.0
captura_camara: /usr/local/lib/libopencv_ml.so.4.8.0
captura_camara: /usr/local/lib/libopencv_objdetect.so.4.8.0
captura_camara: /usr/local/lib/libopencv_photo.so.4.8.0
captura_camara: /usr/local/lib/libopencv_stitching.so.4.8.0
captura_camara: /usr/local/lib/libopencv_video.so.4.8.0
captura_camara: /usr/local/lib/libopencv_videoio.so.4.8.0
captura_camara: /home/fabian/mediapipe/mediapipe/lib/libmediapipe_core.so
captura_camara: /home/fabian/mediapipe/mediapipe/lib/libmediapipe_framework.so
captura_camara: /home/fabian/mediapipe/mediapipe/lib/libmediapipe_calculators.so
captura_camara: /usr/lib/x86_64-linux-gnu/libglog.so.0.6.0
captura_camara: /usr/lib/x86_64-linux-gnu/libgflags.so.2.2.2
captura_camara: /usr/local/lib/libopencv_imgcodecs.so.4.8.0
captura_camara: /usr/local/lib/libopencv_dnn.so.4.8.0
captura_camara: /usr/local/lib/libopencv_calib3d.so.4.8.0
captura_camara: /usr/local/lib/libopencv_features2d.so.4.8.0
captura_camara: /usr/local/lib/libopencv_flann.so.4.8.0
captura_camara: /usr/local/lib/libopencv_imgproc.so.4.8.0
captura_camara: /usr/local/lib/libopencv_core.so.4.8.0
captura_camara: CMakeFiles/captura_camara.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable captura_camara"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/captura_camara.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/captura_camara.dir/build: captura_camara
.PHONY : CMakeFiles/captura_camara.dir/build

CMakeFiles/captura_camara.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/captura_camara.dir/cmake_clean.cmake
.PHONY : CMakeFiles/captura_camara.dir/clean

CMakeFiles/captura_camara.dir/depend:
	cd "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking" "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking" "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build" "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build" "/home/fabian/Documents/Python codes/kinect/kinectcpp/KinectTacking/build/CMakeFiles/captura_camara.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/captura_camara.dir/depend

