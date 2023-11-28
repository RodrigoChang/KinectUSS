import zmq
import cv2
from array import array
import numpy as np
context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://10.171.30.186:5557")
socket.setsockopt_string(zmq.SUBSCRIBE, "")
#enviar x,y
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://0.0.0.0:5558")

def position_frame(x,y):
    socket.send_string(f"{x},{y}")    
    frame_data = socket.recv()
    resolution =(512,424)
    depth_frame = array("f",frame_data)
    # Assuming the depth frame is sent as a 1D array of floats
    frame = np.frombuffer(depth_frame, dtype=np.float32)
    depth_frame_reshape = frame.reshape(resolution)
    valor=depth_frame_reshape[x][y]
    return valor