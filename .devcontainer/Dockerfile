# .devcontainer/Dockerfile
FROM python:3.10

# Set up X11 forwarding
RUN apt-get update && apt-get install -y \
    libsm6 \
    libxext6 \
    libxrender-dev \
    && rm -rf /var/lib/apt/lists/*

# Install OpenCV dependencies
RUN apt-get update && apt-get install -y \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

# Set up a non-root user
RUN useradd -ms /bin/bash vscode
USER vscode

# Set the working directory
WORKDIR /workspace

# Install Python dependencies
COPY .. .
RUN pip install --no-cache-dir mediapipe opencv-python opencv-contrib-python
