import cv2
import zmq
import pickle
import struct

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://0.0.0.0:3001")  # Cambiado el puerto a 3001

cap = cv2.VideoCapture(0)

try:
    while True:
        ret, frame = cap.read()

        # Serializa el frame usando pickle y obtén su longitud
        data = pickle.dumps(frame)
        message_size = struct.pack("L", len(data))

        # Envía la longitud del mensaje y luego el mensaje
        socket.send_multipart([b'', message_size + data])

        # Imprime el tamaño del frame (puedes personalizar esto según tus necesidades)
        print(f"Tamaño del frame enviado: {len(data)} bytes")

        # Espera un tiempo antes de capturar el siguiente frame
        #cv2.imshow('frame', frame)
        cv2.waitKey(100)
except KeyboardInterrupt:
    # Maneja la interrupción de teclado (Ctrl+C) para cerrar la conexión y liberar la cámara
    socket.close()
    cap.release()
