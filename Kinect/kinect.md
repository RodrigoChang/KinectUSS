# Proyectokinect
Este es el proyecto kinect

## Repositorio principal
- [libfreenect2 de OpenKinect](https://github.com/OpenKinect/libfreenect2)

## Proyectos
- [KinectOneStream de chihyaoma](https://github.com/chihyaoma/KinectOneStream)
- [kinect2-opencv de m6c7l](https://github.com/m6c7l/kinect2-opencv)
- [kinect2_tracker](https://github.com/mcgi5sr2/kinect2_tracker)
- [obs-kinect de SirLynix](https://github.com/SirLynix/obs-kinect)

## Dependencias
- OpenGL
- OpenCL
- OpenNI2 (Opcional)
- CUDA (opcional)

## Instalacion en linux
### Usando el codigo original de freenect2 (Recomendado)
Clonar el repositorio
``` bash
git clone https://github.com/OpenKinect/libfreenect2.git
cd libfreenect2
```
Reemplazar ``CL_ICDL_VERSION`` a ``CL_ICDL_VER`` en:
``` bash
src/opencl_depth_packet_processor.cpp:254
src/opencl_depth_packet_processor.cpp:272
src/opencl_kde_depth_packet_processor.cpp:262
src/opencl_kde_depth_packet_processor.cpp:280
```
Esto hara que pueda compilar con OpenCL

### Opcional 
Es posible utilizar stremear/recorder de libfreenect2. Luego de agregarlo al cmake, reemplazar ``CV_IMWRITE_JPEG_QUALITY`` a ``cv::IMWRITE_JPEG_QUALITY`` en los siguientes archivos:
``` bash
tools/streamer_recorder/streamer.cpp:39
tools/streamer_recorder/recorder.cpp:81
```
Esto actualizara la compatibilidad del ProtonectSR a OpenCV 4

Luego asignar una ip y puerto en el archivo config.h:
``` bash
tools/streamer_recorder/include/config.h
define SERVER_ADDRESS "127.0.0.1" // Server IP adress
define SERVER_PORT "10000"    
```

Finalmente se pueden seguir las instrucciones oficiales de libfreenect2 para poder compilar y usar.

### Clonando este repositorio
Es posible compilar o usar los binarios en este repositorio. Prueba tu suerte

``` bash
git clone https://github.com/ctellol/KinectUSS.git
cd Kinect/libfreenect2
```
## Continuando

Ahora podemos dirigirnos a [Corriendo Kinect](../doc/correr_kinect.md) para continuar con la Kinect, o a [MediaPipe](../kinectia/mediapipe.md) para instalarlo si no lo tenemos.

## Troubleshooting

Es posible que no compile por ``ISO C++17 does not allow dynamic exception specifications``, si esto pasa, simplemente puedes poner la flag CMAKE "ENABLE_CXX11" ``-DENABLE_CXX11=ON`` o cambiar la opcion en el CMakeLists.txt

## Eso
Voh dale!
