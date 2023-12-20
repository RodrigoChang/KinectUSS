FROM mcr.microsoft.com/devcontainers/miniconda:0-3

# Copy environment.yml (if found) to a temp location so we update the environment. Also
# copy "noop.txt" so the COPY instruction does not fail if no environment.yml exists.
WORKDIR /app
# [Optional] Uncomment to install a different version of Python than the default
 #RUN conda install -y python=3.11
 #RUN pip install --no-cache-dir pipx
 #RUN pipx reinstall-all
#RUN pip install opencv-python PyOpenGL requests urllib3 certifi charset-normalizer Cython mediapipe zmq
# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive
RUN apt-get -y install --no-install-recommends libsm6 libxext6 libxrender-dev libopencv-dev libqt5gui5
## change to vscode user
USER vscode
RUN conda install -y python=3.11
RUN pip install opencv-python PyOpenGL requests urllib3 certifi charset-normalizer Cython mediapipe zmq
