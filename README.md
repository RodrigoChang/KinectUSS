# KinectUSS
Proyecto basado en el bodytracking, utilizando la Kinect como cámara junto a diversas librerias tales como OpenCV, MediaPipe y Python.

https://github.com/ctellol/KinectUSS/assets/102624847/979460d9-7f28-452d-a14e-0bf865e7de0c

https://github.com/ctellol/KinectUSS/assets/102624847/12ddbc10-a729-4def-8c80-8088356c3c58

![BodyTracking 1](https://github.com/ctellol/KinectUSS/assets/102624847/56c49979-c6c3-4082-bf60-5368d564162f)
![Streaming test1](https://github.com/ctellol/KinectUSS/assets/102624847/6f5eb25e-3ece-4048-81cd-e88291db8ca3)
![Streaming test 2](https://github.com/ctellol/KinectUSS/assets/102624847/f193925a-3bed-455d-a1b7-77558fa4f862)

Instalación:

  Paso 1: Instalacion libfreenect2:
  
  - Windows/Visual Studio:
      - Utilizando "vcpkg" se realiza una instalacion mucho mas sencilla y limpia:

            git clone https://github.com/Microsoft/vcpkg.git
            cd vcpkg
            ./vcpkg integrate install
            vcpkg install libfreenect2
      
  - Linux:

    Nota: Ubuntu 12.04 es demasiado antigua para utilizarla. Debian jessie puede ser tambien demasiado antigua y Debian stretch esta incluida en las instrucciones.

      - Descarga libfreenect2
    
            git clone https://github.com/OpenKinect/libfreenect2.git
            cd libfreenect2

      - (Ubuntu 14.04 only) Descarga actualizacion archivos deb:
        
            cd depends; ./download_debs_trusty.sh

      - Instalacion "build tools":

            sudo apt-get install build-essential cmake pkg-config

      - Instalacion libusb. la version debe ser >= 1.0.20.

        - (Ubuntu 14.04 only)

              sudo dpkg -i debs/libusb*deb
          
        - (Otra)
      
                sudo apt-get install libusb-1.0-0-dev

      
  - MacOS:
    
    - Descarga de libfreenect2:
    
          git clone https://github.com/OpenKinect/libfreenect2.git
          cd libfreenect2
      
    - Instalación dependencias:

          brew update
          brew install libusb
          brew install glfw3
      
    - Build:

          mkdir build && cd build
          cmake ..
          make
          make install
    
  Paso 2:
    - bbb
    
  Paso 3:
    - cccc

Librerias utilizadas:
  - https://github.com/google/mediapipe
  - https://github.com/CMU-Perceptual-Computing-Lab/openpose
  - https://github.com/OpenKinect/libfreenect2
