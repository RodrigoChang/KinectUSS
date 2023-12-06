from flask import Flask, request
import numpy as np
import cv2
import MediaPipe.Servicio.frame_pb2 as frame_pb2
import base64

app = Flask(__name__)

@app.route('/', methods=['POST'])
def receive_frame():
    data = request.data
    # Base64 decode the data
    decoded_data = base64.b64decode(data)

    frame_msg = frame_pb2.Frame()
    frame_msg.ParseFromString(decoded_data)

    rows = frame_msg.rows
    cols = frame_msg.cols
    type = frame_msg.type
    data = np.array(frame_msg.data, dtype=np.uint8).reshape(rows, cols, type)

    cv2.imshow('Received Frame', data)
    cv2.waitKey(1)

    return 'Frame received successfully'

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=6000)
