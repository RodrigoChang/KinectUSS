import cv2
import zmq
import base64
import numpy as np
def server_connect():
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://10.170.50.131:3002")  # Asegúrate de usar la misma dirección y puerto que el servidor
    #socket.connect("tcp://0.0.0.0:3002")
    socket.setsockopt_string(zmq.SUBSCRIBE, '')
    
    while True:
        msg = socket.recv()
        img = base64.b64decode(msg)
        npimg = np.frombuffer(img, dtype=np.uint8)
        frame = cv2.imdecode(npimg, 1)
    
        cv2.imshow("Received Frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
    cv2.destroyAllWindows()
