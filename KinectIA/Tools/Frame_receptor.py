import cv2
import zmq
import numpy as np
import argparse

default_IP = "10.170.53.139"
default_port = "5555"

def server_connect():
    parser = argparse.ArgumentParser(description='sUs.')
    parser.add_argument('--ip', type=str, help='ip del servidor')
    parser.add_argument('-p', type=str, help='puerto del servidor')
    parser.add_argument('--ir', action='store_true', help='Setea el frame como ir')
    args = parser.parse_args()

    server_ip = args.ip if args.ip != None else default_IP
    port = args.p if args.p != None else default_port
    ir = bool(args.ir)
    
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect(f"tcp://{server_ip}:{port}") 
    socket.setsockopt_string(zmq.SUBSCRIBE, '')
    
    while True:
        msg = socket.recv()
        npimg = np.frombuffer(msg, dtype=np.uint8) if ir == False else np.frombuffer(msg, dtype=np.float32)
        frame = cv2.imdecode(npimg, 1)
    
        cv2.imshow("Received Frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break    
    cv2.destroyAllWindows()

if __name__ == "__main__":
    server_connect()