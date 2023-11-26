# Version 3.0

#Distance detecion hands 2.7 mt
import cv2
import mediapipe as mp
from threading import Thread
import time
import numpy as np
from math import acos, degrees

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
resolution =(512,424)
#resolution =(800,600)
class ThreadedCamera(object):
    def __init__(self, src=0):
        #Capturar por camara ,descomentar para usar camara.., comentar para usar streaming 
        self.capture = cv2.VideoCapture(src, cv2.CAP_V4L)
        #Capturar camara Kinect via streaming, descomentar para usar streaming,comentar para usar camara, 
        #self.capture = cv2.VideoCapture(src)
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        confianza = 0.9
        #Parametros Hands
        self.hands = mp_hands.Hands(static_image_mode=False, min_detection_confidence=confianza,min_tracking_confidence=confianza,model_complexity=1)

    def update(self):
        while True:
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)
    
    def process_hands(self, frame):
        frame_resized = cv2.resize(frame, resolution)
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.hands.process(frame_rgb)
        
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                
                """for index, landmark in enumerate(hand_landmarks.landmark):
                    x, y, z = landmark.x, landmark.y, landmark.z
                    cv2.putText(frame_resized, f"({x:.2f},{y:.2f},{z:.2f})", (int(x * frame_resized.shape[1]), int(y * frame_resized.shape[0])),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 0), 2, cv2.LINE_AA)"""
                    
            mp_drawing.draw_landmarks(
                frame_resized, hand_landmarks, mp_hands.HAND_CONNECTIONS,
                mp_drawing.DrawingSpec(color=(0, 0, 0), thickness=2),
                mp_drawing.DrawingSpec(color=(0, 0, 0), thickness=2))
            
        return frame_resized


    def show_frame(self):
        if self.frame is not None:
            processed_frame = self.process_hands(processed_frame)
            cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)

if __name__ == '__main__':
    src = 0
    #src = 'udp://0.0.0.0:6000?overrun_nonfatal=1'
    threaded_camera = ThreadedCamera(src)
    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
