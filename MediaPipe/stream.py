import cv2
import zmq
import numpy as np

depth_frame = None

RGB = True
IR = True
DEPTH = True
REGISTERED = True

IP = "127.0.0.1"

def receive_depth_frame(socket):
    global depth_frame
    frame_bytes = socket.recv()
    depth_frame = np.frombuffer(frame_bytes, dtype='float32')
    depth_frame = depth_frame.reshape((height, width))
    return depth_frame

def receive_frame(socket):
    frame_bytes = socket.recv()
    frame = np.frombuffer(frame_bytes, dtype='uint8')
    frame = frame.reshape((height, width))
    return frame

def mouse_callback(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        if depth_frame is not None and 0 <= y < height and 0 <= x < width:
            depth_value = depth_frame[y, x]
            print(f"Profundidad en el pixel ({x}, {y}): {depth_value} mm")

def main():
    global height, width

    context = zmq.Context()
    socketrgb = context.socket(zmq.SUB)
    socketir = context.socket(zmq.SUB)
    socketdepth = context.socket(zmq.SUB)
    socketreg = context.socket(zmq.SUB)

    socketrgb.connect(f'tcp://{IP}:5555') 
    socketir.connect(f'tcp://{IP}:5556')
    socketdepth.connect(f'tcp://{IP}:5557')  
    socketreg.connect(f'tcp://{IP}:5558') 

    socketrgb.subscribe(b'')
    socketir.subscribe(b'')
    socketdepth.subscribe(b'')
    socketreg.subscribe(b'')

    cv2.namedWindow("Depth")
    cv2.setMouseCallback("Depth", mouse_callback)

    while True:
        rgb_frame = receive_frame(socketrgb)
        ir_frame = receive_frame(socketir)
        depth_frame = receive_depth_frame(socketdepth)
        reg_frame = receive_frame(socketreg)
        cv2.imshow("RGB", rgb_frame)
        cv2.imshow("IR", ir_frame / 4096.0)
        cv2.imshow("Depth", depth_frame / 4096.0)
        cv2.imshow("Depth", reg_frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    socketrgb.close()
    socketir.close()
    socketdepth.close()
    socketreg.close()
    context.term()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    height, width = 424, 512
    main()