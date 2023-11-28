import zmq
import numpy as np
import cv2

def receive_frames():
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://10.170.22.235:5556")  # Match the address used in the C++ program

    # Subscribe to all topics
    socket.setsockopt_string(zmq.SUBSCRIBE, "")

    while True:
        # Receive the message
        message = socket.recv()
        #print(message)
        # Convert the received bytes to a NumPy array
        frame_data = np.frombuffer(message, dtype=np.float32)
        #frame = cv2.imdecode(frame_data, cv2.IMREAD_COLOR)
        frame_data = cv2.resize(frame_data,(512,424))
        

        # Assuming the frame is in BGR format, reshape it to the original shape

        #height, width, channels = 512, 424, 4
        # Change these values based on your Kinect settings
        #cv2.
        #frame = frame_data.reshape((height, width, channels))

        # Process the received frame (for example, display it using OpenCV)
        #cv2.imshow("color", frame)
        cv2.imshow("color", frame_data/1024)
        cv2.waitKey(1)
        #return frame
        
receive_frames()