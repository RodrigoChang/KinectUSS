# KinectUSS
Proyecto basado en el bodytracking, utilizando la Kinect como cámara junto a diversas librerias tales como libfreenect2, MediaPipe y Python.

  Busca trackear los movimientos del individuo, calculando el angulo de movimiento de sus extremidades para poder detectar si el movimiento deseado esta realizado de manera correcta o si existe un margen de error del individuo.
  
https://github.com/ctellol/KinectUSS/assets/102624847/979460d9-7f28-452d-a14e-0bf865e7de0c

https://github.com/ctellol/KinectUSS/assets/102624847/12ddbc10-a729-4def-8c80-8088356c3c58

![Streaming test1](https://github.com/ctellol/KinectUSS/assets/102624847/6f5eb25e-3ece-4048-81cd-e88291db8ca3)
![Streaming test 2](https://github.com/ctellol/KinectUSS/assets/102624847/f193925a-3bed-455d-a1b7-77558fa4f862)

Nota: Se recomienda utilizar Visual Studio Code para mayor comodidad.

Instalación:

Nota: Para un correcto funcionamiento del programa como tal, es necesario hacer las instalaciones de las librerias y dependencias necesarias que dependeran del sistema operativo en que se desee implementar. Es por esto que se separaron en 2 pasos de manera general:

  Paso 1: Instalacion libfreenect2:
  
  - Windows/Visual Studio:
      - Utilizando "vcpkg" se realiza una instalacion mucho mas sencilla y limpia:

            git clone https://github.com/Microsoft/vcpkg.git
            cd vcpkg
            ./vcpkg integrate install
            vcpkg install libfreenect2
      
  - Linux:

    Nota: Ubuntu 12.04 es demasiado antigua para utilizarla. Debian jessie puede ser tambien demasiado antigua y Debian stretch esta incluida en las instrucciones.

      - Descarga libfreenect2:
    
            git clone https://github.com/OpenKinect/libfreenect2.git
            cd libfreenect2

      - (Ubuntu 14.04 only) Descarga actualizacion archivos deb:
        
            cd depends; ./download_debs_trusty.sh

      - Instalacion "build tools":

            sudo apt-get install build-essential cmake pkg-config

      - Instalacion libusb. la version debe ser >= 1.0.20.

        - Ubuntu 14.04 only:

              sudo dpkg -i debs/libusb*deb
          
        - Otras:
      
              sudo apt-get install libusb-1.0-0-dev
          
      - Instalacion TurboJPEG:
        
        - Ubuntu 14.04 a 16.04:
          
              sudo apt-get install libturbojpeg libjpeg-turbo8-dev

        - Debian/Ubuntu 17.10 y nuevas:
        
              sudo apt-get install libturbojpeg0-dev
          
      - Instalacion OpenGL:
        
        - Ubuntu 14.04 only:
          
              sudo dpkg -i debs/libglfw3*deb; sudo apt-get install -f
          
        - (Odroid XU4) OpenGL 3.1 no tiene soporte para esta plataforma. Usa:

              cmake -DENABLE_OPENGL=OFF later.
        
        - Otras:
          
              sudo apt-get install libglfw3-dev

      - Instalacion OpenCL (opcional)

        - Intel GPU:

          - Ubuntu 14.04 only:
            
                  sudo apt-add-repository ppa:floe/beignet; sudo apt-get update; sudo apt-get install beignet-dev; sudo dpkg -i debs/ocl-icd*deb
                
           - Otras:
                 
                  sudo apt-get install beignet-dev
             
        - AMD GPU: 
          
              apt-get install opencl-headers
          
        - Mali GPU (e.g. Odroid XU4): (with root)

              mkdir -p /etc/OpenCL/vendors; echo /usr/lib/arm-linux-gnueabihf/mali-egl/libmali.so >/etc/OpenCL/vendors/mali.icd; apt-get install opencl-headers.

      - Instalacion CUDA (opcional, Nvidia only):
        
          - Ubuntu 14.04 only:
            
                 apt-get install cuda
            
      - Instalacion VAAPI (opcional, Intel only):
  
          - Ubuntu 14.04 only:
            
                sudo dpkg -i debs/{libva,i965}*deb; sudo apt-get install -f
            
          - Otras:
            
                sudo apt-get install libva-dev libjpeg-dev
            
      - Instalacion OpenNI2 (opcional)
        
          - Ubuntu 14.04 only:
            
                sudo apt-add-repository ppa:deb-rob/ros-trusty && sudo apt-get update (You don't need this if you have ROS repos), then sudo apt-get install libopenni2-dev
            
          - Otra:
            
                sudo apt-get install libopenni2-dev
      
  - MacOS:
    
    - Descarga de libfreenect2:
    
          git clone https://github.com/OpenKinect/libfreenect2.git
          cd libfreenect2
      
    - Instalación dependencias:

          brew update
          brew install libusb
          brew install glfw3
      
    - Instalacion TurboJPEG (opcional)

          brew install jpeg-turbo

    - Instalacion OpenNI2 (opcional)
      
          brew tap brewsci/science
          brew install openni2
          export OPENNI2_REDIST=/usr/local/lib/ni2
          export OPENNI2_INCLUDE=/usr/local/include/ni2
      
    - Build:

          mkdir build && cd build
          cmake ..
          make
          make install
    
Paso 2: Istalacion Media Pipe en Python
  
  - Antes de iniciar Mediapipe en alguna tarea en python, instala el package de Mediapipe:

        $ python -m pip install mediapipe
    
  - Luego de la instalacion importa a tu proyecto lo siguiente:
    
        import mediapipe as mp
    
  - Para dependencias:

    - Vision:
      
          from mediapipe.tasks.python import vision
      
    - Texto:
   
          from mediapipe.tasks.python import text

    - Audio:
    
          from mediapipe.tasks.python import audio
      
Paso 3: Instalacion de OBS (opcional, solo instalar si se busca ejecutar el codigo de manera remota):

  - Windows:   https://cdn-fastly.obsproject.com/downloads/OBS-Studio-29.1.3-Full-Installer-x64.exe
    
  - Linux:

      - Debian/Ubuntu-based:

            sudo apt install v4l2loopback-dkms
        
      - Red Hat/Fedora-based (requires RPM Fusion to be enabled):

            sudo dnf install kmod-v4l2loopback
        
      - Arch Linux-based/Manjaro:

            sudo pacman -Sy v4l2loopback-dkms
    
  - MacOS:

    - Intel:   https://cdn-fastly.obsproject.com/downloads/obs-studio-29.1.3-macos-x86_64.dmg
      
    - Apple Silicon:   https://cdn-fastly.obsproject.com/downloads/obs-studio-29.1.3-macos-arm64.dmg
      
Librerias utilizadas:

  - https://github.com/google/mediapipe
  
  - https://github.com/OpenKinect/libfreenect2
