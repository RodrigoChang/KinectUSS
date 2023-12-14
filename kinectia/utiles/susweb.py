import zmq
import cv2
from array import array
import numpy as np
context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://10.171.23.91:5557")
#enviar x,y
socket2 = context.socket(zmq.PUB)
socket2.bind("tcp://0.0.0.0:5558")

def position_frame(x,y):
    socket2.send_string(f"{x},{y}")
    socket.setsockopt_string(zmq.SUBSCRIBE, '')    
    frame_data = socket.recv()
    resolution =(512,424)
    depth_frame = array("f",frame_data)
    # Assuming the depth frame is sent as a 1D array of floats
    frame = np.frombuffer(depth_frame, dtype=np.float32)
    depth_frame_reshape = frame.reshape(resolution)
    valor=depth_frame_reshape[x][y]
    socket.setsockopt_string(zmq.UNSUBSCRIBE, '')
    return valor