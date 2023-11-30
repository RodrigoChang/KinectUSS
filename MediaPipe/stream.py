import cv2
import zmq
import numpy as np

depth_frame = None

RGB = True
IR = True
DEPTH = True
REGISTERED = True

IP = "127.0.0.1"
context = zmq.Context()

def receive_rgb_frame():
    socketrgb = context.socket(zmq.SUB)
    socketrgb.setsockopt(zmq.SUBSCRIBE, b"")
    socketrgb.connect(f'tcp://{IP}:5555')
    frame_bytes = socketrgb.recv()
    rgb_frame = np.frombuffer(frame_bytes, dtype='uint8')
    rgb_frame = cv2.imdecode(rgb_frame, cv2.IMREAD_COLOR)
    socketrgb.close()
    return rgb_frame

def receive_32bit_frame(socket):
    frame_bytes = socket.recv()
    frame = np.frombuffer(frame_bytes, dtype='float32')
    frame = frame.reshape((height, width))
    socket.close()
    return frame

def receive_ir_frame():
    global depth_frame
    socketir = context.socket(zmq.SUB)
    socketir.setsockopt(zmq.SUBSCRIBE, b"")
    socketir.connect(f'tcp://{IP}:5556') 
    frame_bytes = socketir.recv()
    ir_frame = np.frombuffer(frame_bytes, dtype='float32')
    ir_frame = ir_frame.reshape((height, width))
    socketir.close()
    return ir_frame

def receive_depth_frame():
    socketdepth = context.socket(zmq.SUB)
    socketdepth.setsockopt(zmq.SUBSCRIBE, b"")
    socketdepth.connect(f'tcp://{IP}:5557') 
    frame_bytes = socketdepth.recv()
    depth_frame = np.frombuffer(frame_bytes, dtype='float32')
    depth_frame = depth_frame.reshape((height, width))
    socketdepth.close()
    return depth_frame

def receive_reg_frame():
    socketreg = context.socket(zmq.SUB)
    socketreg.setsockopt(zmq.SUBSCRIBE, b"")
    socketreg.connect(f'tcp://{IP}:5558')
    frame_bytes = socketreg.recv()
    reg_frame = np.frombuffer(frame_bytes, dtype='uint8')
    reg_frame = cv2.imdecode(reg_frame, cv2.IMREAD_COLOR)
    socketreg.close()
    return reg_frame

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
        rgb_frame = receive_rgb_frame()
        ir_frame = receive_ir_frame()
        depth_frame = receive_depth_frame()
        reg_frame = receive_reg_frame()
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