import cv2
import zmq
import numpy as np

depth_frame = None
framecount = 0

IP = "127.0.0.1"
context = zmq.Context()
socketrgb = context.socket(zmq.SUB)
socketir = context.socket(zmq.SUB)
socketdepth = context.socket(zmq.SUB)
socketreg = context.socket(zmq.SUB)

#socketrgb.setsockopt(zmq.CONFLATE, 1)
#socketir.setsockopt(zmq.CONFLATE, 1)
#socketdepth.setsockopt(zmq.CONFLATE, 1)
#socketreg.setsockopt(zmq.CONFLATE, 1)

socketrgb.connect(f'tcp://{IP}:5555')
socketir.connect(f'tcp://{IP}:5556') 
socketdepth.connect(f'tcp://{IP}:5557') 
socketreg.connect(f'tcp://{IP}:5558')

socketrgb.setsockopt(zmq.SUBSCRIBE, b'')
socketir.setsockopt(zmq.SUBSCRIBE, b'')
socketdepth.setsockopt(zmq.SUBSCRIBE, b'')
socketreg.setsockopt(zmq.SUBSCRIBE, b'')

def receive_rgb_frame():
    frame_bytes = socketrgb.recv()
    rgb_frame = np.frombuffer(frame_bytes, dtype='uint8')
    rgb_frame = cv2.imdecode(rgb_frame, cv2.IMREAD_COLOR)
    return rgb_frame

def receive_ir_frame():
    frame_bytes = socketir.recv()
    ir_frame = np.frombuffer(frame_bytes, dtype='float32')
    ir_frame = ir_frame.reshape((height, width))
    return ir_frame

def receive_depth_frame():
    global depth_frame
    frame_bytes = socketdepth.recv()
    depth_frame = np.frombuffer(frame_bytes, dtype='float32')
    depth_frame = depth_frame.reshape((height, width))
    return depth_frame

def receive_reg_frame():
    frame_bytes = socketreg.recv()
    reg_frame = np.frombuffer(frame_bytes, dtype='uint8')
    reg_frame = cv2.imdecode(reg_frame, cv2.IMREAD_COLOR)
    return reg_frame

def mouse_callback(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        if depth_frame is not None and 0 <= y < height and 0 <= x < width:
            depth_value = depth_frame[y, x]
            print(f"Profundidad en el pixel ({x}, {y}): {depth_value} mm")

def main():
    global height, width
    global framecount
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