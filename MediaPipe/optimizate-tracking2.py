import cv2
import mediapipe as mp
from threading import Thread
import time

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

class ThreadedCamera(object):
    def __init__(self, src=0):
        self.capture = cv2.VideoCapture(src)
        self.capture.set(cv2.CAP_PROP_BUFFERSIZE, 2)
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        self.pose = mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.90)
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

    def show_frame(self):
        processed_frame = self.process_pose(self.frame)
        cv2.imshow('frame', processed_frame)
        cv2.waitKey(self.FPS_MS)

if __name__ == '__main__':
    src = 'udp://0.0.0.0:6000?overrun_nonfatal=1'
    threaded_camera = ThreadedCamera(src)

    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
