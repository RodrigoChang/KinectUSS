# Importaciones necesarias
import cv2
import mediapipe as mp
from threading import Thread
import time
import numpy as np

# Inicialización de Mediapipe
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_holistic = mp.solutions.holistic

# Clase para la cámara y procesamiento
class ThreadedCamera(object):
    def __init__(self, src=0):
        # Inicialización de la cámara
        self.capture = cv2.VideoCapture(src, cv2.CAP_V4L)
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        
        # Inicialización de Mediapipe Holistic
        self.holistic = mp_holistic.Holistic(
            static_image_mode=False, min_detection_confidence=0.70
        )

    def update(self):
        while True:
            # Captura de fotogramas
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)

    def process_pose(self, frame):
        # Procesamiento de la pose con Mediapipe Holistic
        frame_resized = cv2.resize(frame, (800, 600))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results_holistic = self.holistic.process(frame_rgb)

        if results_holistic.pose_landmarks:
            pose_landmarks = results_holistic.pose_landmarks.landmark

            # Dibujar landmarks de pose
            for landmark in pose_landmarks:
                x, y = int(landmark.x * frame_resized.shape[1]), int(landmark.y * frame_resized.shape[0])
                cv2.circle(frame_resized, (x, y), 5, (128, 0, 250), -1)

            # Dibujar conexiones de pose
            mp_drawing.draw_landmarks(
                frame_resized,
                results_holistic.pose_landmarks,
                mp_holistic.POSE_CONNECTIONS,
                landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style()
            )

        return frame_resized

    def show_frame(self):
        # Mostrar el fotograma procesado en una ventana
        if self.frame is not None:
            processed_frame = self.process_pose(self.frame)
            cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)

# Función principal
if __name__ == '__main__':
    # Configuración de la cámara
    src = 0
    threaded_camera = ThreadedCamera(src)

    # Bucle principal
    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
