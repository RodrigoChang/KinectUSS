import cv2
import zmq
import numpy as np

depth_frame = None

def receive_depth_frame(socket):
    global depth_frame
    frame_bytes = socket.recv()
    depth_frame = np.frombuffer(frame_bytes, dtype='float32')
    depth_frame = depth_frame.reshape((height, width))
    return depth_frame

def mouse_callback(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        if depth_frame is not None and 0 <= y < height and 0 <= x < width:
            depth_value = depth_frame[y, x]
            print(f"Profundidad en el pixel ({x}, {y}): {depth_value} mm")

def main():
    global height, width

    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://127.0.0.1:5557")  
    socket.subscribe(b'')

    cv2.namedWindow("Depth")
    cv2.setMouseCallback("Depth", mouse_callback)

    while True:
        depth_frame = receive_depth_frame(socket)
        cv2.imshow("Depth", depth_frame / 4096.0)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    socket.close()
    context.term()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    height, width = 424, 512
    main()