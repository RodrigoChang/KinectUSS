import cv2
import mediapipe as mp
from threading import Thread
import time

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands

class ThreadedCamera(object):
    def __init__(self, src=0):
        self.capture = cv2.VideoCapture(src, cv2.CAP_V4L)  # Cambio en la inicialización
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        self.pose = mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.90)
        self.hands = mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.7)
        self.up = False
        self.down = False
        self.count = 0

    def update(self):
        while True:
            if self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)

    def process_pose(self, frame):
        # Redimensionar la imagen a 800x600
        frame_resized = cv2.resize(frame, (800, 600))

        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.pose.process(frame_rgb)

        if results.pose_landmarks is not None:
            mp_drawing.draw_landmarks(
                frame_resized, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
                mp_drawing.DrawingSpec(color=(128, 0, 250), thickness=2, circle_radius=3),
                mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=2))

        return frame_resized

    def process_hands(self, frame):
        frame_resized = cv2.resize(frame, (800, 600))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.hands.process(frame_rgb)

        if results.multi_hand_landmarks:
            for landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    frame_resized, landmarks, mp_hands.HAND_CONNECTIONS,
                    mp_drawing.DrawingSpec(color=(0, 128, 255), thickness=2),
                    mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=2))
                
        return frame_resized

    def show_frame(self):
        processed_frame = self.process_pose(self.frame)
        processed_frame = self.process_hands(processed_frame)
        cv2.imshow('frame', processed_frame)
        cv2.waitKey(self.FPS_MS)

if __name__ == '__main__':
    src = 0  # Usando la fuente 0 por defecto (cámara)

    threaded_camera = ThreadedCamera(src)

    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
