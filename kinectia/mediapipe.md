# MediaPipe

MediaPipe en una libreria de google en python3.x, asi que lo primero que haremos es instalar python y las dependencias.

## En Linux

Descargar Anaconda para linux:
´´´bash
"Anaconda": https://repo.anaconda.com/archive/Anaconda3-2023.09-0-Linux-x86_64.sh
"usar en terminal": bash Anaconda-latest-Linux-x86_64.sh
´´´
Crear un entorno python
´´´bash
Conda create --name <Nombre del entorno> python=<3.8>
´´´
Inicializar
´´´bash
Conda activate <Nombre del entorno>
´´´
# Librerias necesarias

Instalar python y ffmpeg.
```bash
# Debian:
sudo apt install python3 ffmpeg
# Red Hat-based:
sudo dnf install python3 ffmpeg
# Arch(btw):
sudo pacman -S python3 ffmpeg
```
Instalar librerias de python.
```bash
pip install opencv-python numpy mediapipe 
```

## En Windows
Instalar python y ffmpeg.
- [Python](https://www.python.org/downloads/release/python-3113/), Asegurate de hacer click en avanzado y ticker  Add Python to PATH"
- [FFmpeg](https://ffmpeg.org/download.html)
Instalar librerias de python.
```bash
pip install mediapipe opencv-python
```

## Continuando

Para continuar, visitamos [Corriendo MediaPipe](doc/correr_mediapipe.md), o a [Kinect](Kinect/kinect.md) si no esta instalado.
