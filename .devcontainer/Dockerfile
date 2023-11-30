# Use the official Python image from the Docker Hub
FROM python:3.10

# Set the working directory in the container
WORKDIR /app

# Copy the requirements file into the container
#COPY requirements.txt .

# Install any needed packages specified in requirements.txt
RUN pip install --no-cache-dir mediapipe opencv-python opencv-contrib-python

# Copy the local code into the container
COPY . /app
