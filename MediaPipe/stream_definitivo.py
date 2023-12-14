import cv2
import zmq
import numpy as np

framecount = 0
IP = "0.0.0.0"

first_value = 183
second_value = 358
message = f"{first_value},{second_value}".encode('utf-8')

context = zmq.Context()
socketrgb = context.socket(zmq.SUB)
socketir = context.socket(zmq.SUB)
socket_z = context.socket(zmq.REQ)
socketreg = context.socket(zmq.SUB)

socketrgb.bind(f'tcp://{IP}:5555')
socketir.bind(f'tcp://{IP}:5556') 
socket_z.bind(f'tcp://{IP}:5557')
socketreg.bind(f'tcp://{IP}:5558')

socketrgb.setsockopt(zmq.SUBSCRIBE, b'')
socketir.setsockopt(zmq.SUBSCRIBE, b'')
socketreg.setsockopt(zmq.SUBSCRIBE, b'')

##recieve
def receive_8bit(socket):
    frame_bytes = socket.recv()
    frame_size = len(frame_bytes)
    img_frame = np.frombuffer(frame_bytes, dtype='uint8')
    img_frame = cv2.imdecode(img_frame, cv2.IMREAD_COLOR)
    return img_frame, frame_size

def ask_z(x, y):
    message = f"{x},{y}".encode('utf-8')
    socket_z.send(message)
    response_msg = socket_z.recv()
    print(f"Received response: {response_msg.decode('utf-8')}")

def main():
    global height, width, framecount
    global socketrgb, socketir, socketreg

    while True:
        rgb_frame, rgb_size = receive_8bit(socketrgb)
        ir_frame, ir_size = receive_8bit(socketir)
        reg_frame, reg_size = receive_8bit(socketreg)
        socket_z.send(message)

        response = socket_z.recv()

        print(f"RGB Frame Size: {rgb_size} bytes")
        print(f"IR Frame Size: {ir_size} bytes")
        print(f"Registered Frame Size: {reg_size} bytes")
        print("Respuesta:", response.decode('utf-8'))

        cv2.imshow("RGB", rgb_frame)
        cv2.imshow("IR", ir_frame)
        cv2.imshow("Registered", reg_frame)

        framecount = framecount + 1
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        if (framecount == 10):
            #unsubscribe
            socketrgb.setsockopt(zmq.UNSUBSCRIBE, b'')
            socketir.setsockopt(zmq.UNSUBSCRIBE, b'')
            socketreg.setsockopt(zmq.UNSUBSCRIBE, b'')
            #subscribe
            socketrgb.setsockopt(zmq.SUBSCRIBE, b'')
            socketir.setsockopt(zmq.SUBSCRIBE, b'')
            socketreg.setsockopt(zmq.SUBSCRIBE, b'')
            framecount = 0
    context.term()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    height, width = 424, 512
    main()