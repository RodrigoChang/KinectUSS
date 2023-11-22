import cv2
import mediapipe as mp
import numpy as np
import threading as th
from math import acos, degrees

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

#cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
cap = cv2.VideoCapture('udp://0.0.0.0:6000?overrun_nonfatal=1')
cap.set(cv2.CAP_PROP_BUFFERSIZE, 2)
cap.set(cv2.CAP_PROP_FPS, 60)
up = False
down = False
count = 0

with mp_pose.Pose(
    static_image_mode=False,
    min_detection_confidence=0.90) as pose:
    while True:
        ret, frame = cap.read()

        if not ret:
            break

        frame = cv2.flip(frame, 1)
        frame = cv2.resize(frame, (800, 600))
        height, width, _ = frame.shape
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = pose.process(frame_rgb)
        
        if results.pose_landmarks is not None:
            mp_drawing.draw_landmarks(
                frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS,
               
                mp_drawing.DrawingSpec(color=(128, 0, 250), thickness=2, circle_radius=3),
                mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=2))
            #print(results.pose_landmarks)
            #print(mp_pose.POSE_CONNECTIONS)
        cv2.useOptimized() # Returns True if optimisation is enabled 
        cv2.setUseOptimized(True) # Set it to True    
        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0XFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
