# Version 1.0
import cv2
import mediapipe as mp
from threading import Thread
import time

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands

class ThreadedCamera(object):
    def __init__(self, src=0):
        self.capture = cv2.VideoCapture(src, cv2.CAP_V4L)
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
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)

    def process_pose(self, frame):
        frame_resized = cv2.resize(frame, (800, 600))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.pose.process(frame_rgb)

        if results.pose_landmarks is not None:
            # Comprobar si el landmark del hombro derecho (12) y el codo derecho (13) están presentes
            if results.pose_landmarks is not None:  
                right_shoulder_landmark = results.pose_landmarks.landmark[12]
                left_shoulder_landmark = results.pose_landmarks.landmark[11]
                left_elbow_landmark = results.pose_landmarks.landmark[13]
                left_wrist_landmark = results.pose_landmarks.landmark[15]
                
                x1, y1 = int(right_shoulder_landmark.x * frame_resized.shape[1]), int(right_shoulder_landmark.y * frame_resized.shape[0])
                x2, y2 = int(left_shoulder_landmark.x * frame_resized.shape[1]), int(left_shoulder_landmark.y * frame_resized.shape[0])
                x3, y3 = int(left_elbow_landmark.x * frame_resized.shape[1]), int(left_elbow_landmark.y * frame_resized.shape[0])
                x4, y4 = int(left_wrist_landmark.x * frame_resized.shape[1]), int(left_wrist_landmark.y * frame_resized.shape[0])

                # Dibujar un círculo en el punto del hombro derecho
                cv2.circle(frame_resized, (x1, y1), 5, (0, 0, 255), -1)
                cv2.circle(frame_resized, (x2, y2), 5, (0, 0, 255), -1)
                cv2.circle(frame_resized, (x3, y3), 5, (0, 0, 255), -1)
                cv2.circle(frame_resized, (x4, y4), 5, (0, 0, 255), -1)

                # Dibujar una línea entre el hombro derecho y el codo derecho
                cv2.line(frame_resized, (x1, y1), (x2, y2), (0, 0, 255), 2)
                cv2.line(frame_resized, (x2, y2), (x3, y3), (0, 0, 255), 2)
                cv2.line(frame_resized, (x3, y3), (x4, y4), (0, 0, 255), 2) 

                x = right_shoulder_landmark.x
                y = right_shoulder_landmark.y
                z = right_shoulder_landmark.z
                #print(f"Right Shoulder - X: {x}, Y: {y}, Z: {z}")

        return frame_resized

    def process_hands(self, frame):
        frame_resized = cv2.resize(frame, (800, 600))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.hands.process(frame_rgb)

        if results.multi_hand_landmarks:
            for landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    frame_resized, landmarks, mp_hands.HAND_CONNECTIONS,
                    mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2),
                    mp_drawing.DrawingSpec(color=(0, 0, 255), thickness=2))
                
        return frame_resized

    def show_frame(self):
        if self.frame is not None:
            processed_frame = self.process_pose(self.frame)
            processed_frame = self.process_hands(processed_frame)
            cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)

if __name__ == '__main__':
    src = 0

    threaded_camera = ThreadedCamera(src)

    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
