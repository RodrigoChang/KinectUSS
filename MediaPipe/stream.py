import cv2
import zmq
import imagezmq
import numpy as np
import base64

depth_frame = None
IP = "127.0.0.1"
context = zmq.Context()
socketrgb = context.socket(zmq.SUB)
socketir = context.socket(zmq.SUB)
socketdepth = context.socket(zmq.SUB)
socketreg = context.socket(zmq.SUB)

socketrgb.connect(f'tcp://{IP}:5565')
socketir.connect(f'tcp://{IP}:5566') 
socketdepth.connect(f'tcp://{IP}:5567') 
socketreg.connect(f'tcp://{IP}:5568')

socketrgb.setsockopt(zmq.SUBSCRIBE, b'')
socketir.setsockopt(zmq.SUBSCRIBE, b'')
socketdepth.setsockopt(zmq.SUBSCRIBE, b'')
socketreg.setsockopt(zmq.SUBSCRIBE, b'')

"""
socketrgb.setsockopt(zmq.IDENTITY, b'rgb_socket')
socketir.setsockopt(zmq.IDENTITY, b'ir_socket')
socketdepth.setsockopt(zmq.IDENTITY, b'depth_socket')
socketreg.setsockopt(zmq.IDENTITY, b'reg_socket')"""

def receive_8bit(socket):
    frame_bytes = socket.recv()
    img_frame = np.frombuffer(frame_bytes, dtype='uint8')
    img_frame = cv2.imdecode(img_frame, cv2.IMREAD_COLOR)
    return img_frame

def receive_32bit(socket):
    frame_bytes = socket.recv()
    frame_data = base64.b64decode(frame_bytes)
    mat_frame = np.frombuffer(frame_data, dtype='float32')
    mat_frame = mat_frame.reshape((height, width))
    return mat_frame

def mouse_callback(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        if depth_frame is not None and 0 <= y < height and 0 <= x < width:
            depth_value = depth_frame[y, x]
            print(f"Profundidad en el pixel ({x}, {y}): {depth_value} mm")

def main():
    global height, width
    cv2.namedWindow("Depth")
    cv2.setMouseCallback("Depth", mouse_callback)

    while True:
        rgb_frame = receive_8bit(socketrgb)
        ir_frame = receive_32bit(socketir)
        depth_frame = receive_32bit(socketdepth)
        reg_frame = receive_8bit(socketreg)

        cv2.imshow("RGB", rgb_frame)
        cv2.imshow("IR", ir_frame / 4096.0)
        cv2.imshow("Depth", depth_frame / 4096.0)
        cv2.imshow("Registered", reg_frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    context.term()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    height, width = 424, 512
    main()