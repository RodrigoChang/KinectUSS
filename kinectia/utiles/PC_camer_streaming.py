import cv2
import zmq
import base64

def video_stream():
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    # Puedes cambiar la dirección y el puerto según tus necesidades
    socket.bind("tcp://0.0.0.0:3001")  

    cap = cv2.VideoCapture(0)  # Cambia 0 a la fuente de video que estás utilizando

    # Ajusta la resolución deseada
    width, height = 512, 424  # Puedes cambiar estos valores según tus necesidades
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        _, img_encoded = cv2.imencode('.jpg', frame)
        msg = base64.b64encode(img_encoded.tobytes())
        socket.send(msg)

video_stream()

