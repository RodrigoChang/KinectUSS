import zmq
import numpy as np
context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://10.171.22.235:5555")
socket.setsockopt_string(zmq.SUBSCRIBE, "")
def position_frame(x,y):    
    frame_data = socket.recv()
    resolution =(512,424)
    # Assuming the depth frame is sent as a 1D array of floats
    depth_frame = np.frombuffer(frame_data, dtype=np.float32)
    depth_frame_reshape = depth_frame.reshape(resolution)
    valor=depth_frame_reshape[x][y]
    return valor