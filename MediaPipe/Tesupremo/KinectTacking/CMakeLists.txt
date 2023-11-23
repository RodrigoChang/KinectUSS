cmake_minimum_required(VERSION 3.12)
project(TuProyecto)

# Encuentra e incluye las bibliotecas de OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Configura el directorio de instalación de MediaPipe (ajusta la ruta según tu configuración)
set(MEDIAPIPE_INSTALL_DIR "/home/fabian/mediapipe")

# Encuentra e incluye las bibliotecas de MediaPipe
find_library(MEDIAPIPE_LIB mediapipe
    HINTS "${MEDIAPIPE_INSTALL_DIR}/lib"
    REQUIRED)

include_directories("${MEDIAPIPE_INSTALL_DIR}/include")

# Configura el ejecutable de tu proyecto
add_executable(TuProyecto main.cpp)

# Enlaza las bibliotecas necesarias
target_link_libraries(TuProyecto ${OpenCV_LIBS} ${MEDIAPIPE_LIB})
