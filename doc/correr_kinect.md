# Corriendo Kinect

El archivo KinectSUS, contiene un codigo funcional para poder ingresar y stremear las camaras de la Kinect. Este codigo tambien envia los frames de cada camara a una ip para luego usarla en [MediaPipe](doc/correr_mediapipe.md).
## KinectSUS
Primero antes de usar, se debe compilar.
``` bash
cd KinectSUS
mkdir build && cd build
cmake ..
make -j$(nproc)
```
Una ves esto listo, podremos ejecutar el binario.
``` bash
./KinectSUS
```
## Continuando

Si no tienes MediaPipe listo, ve a [MediaPipe](MediaPipe/mediapipe.md) para instalarlo.
