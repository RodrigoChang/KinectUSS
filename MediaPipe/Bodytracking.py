# Version 3.0

#Distance detecion hands 2.7 mt
import cv2
import mediapipe as mp
from threading import Thread
import time
#Comentar para que parta sin socket
#import susweb as sus
import numpy as np
from math import acos, degrees

mp_pose = mp.solutions.pose
#resolution =(512,424)
resolution =(800,600)
class ThreadedCamera(object):
    def __init__(self, src=0):
        #Capturar Camara o Streaming 
        self.capture = src 
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        #Parametros body
        confianza = 0.9
        self.pose =  mp_pose.Pose(static_image_mode=False, min_detection_confidence=confianza,min_tracking_confidence=confianza,model_complexity=2, smooth_landmarks= True)
        
    def update(self):
        while True:
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)
    
    def process_pose(self, frame):
        frame_resized = cv2.resize(frame, resolution)
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results_skeleto = self.pose.process(frame_rgb)
        
        if results_skeleto.pose_landmarks is not None:
                #aqui dentro debes poner tu codigo 
                #Dibujo de body
                #Obtencion de nodos lado izquierdo
                #numbers of landmarks 11,13,15,23,25,27,29,31
                
                left_shoulder_landmark = results_skeleto.pose_landmarks.landmark[11]
                left_elbow_landmark = results_skeleto.pose_landmarks.landmark[13]
                left_wrist_landmark = results_skeleto.pose_landmarks.landmark[15]
                left_hip_landmark = results_skeleto.pose_landmarks.landmark[23]
                left_knee_landmark = results_skeleto.pose_landmarks.landmark[25]
                left_ankle_landmark = results_skeleto.pose_landmarks.landmark[27]
                left_heel_landmark = results_skeleto.pose_landmarks.landmark[29]
                left_foot_index_landmark = results_skeleto.pose_landmarks.landmark[31]
                
                # X and y, Left body 
                x_left_shoulder, y_left_shoulder = int(left_shoulder_landmark.x * frame_resized.shape[1]), int(left_shoulder_landmark.y * frame_resized.shape[0])
                x_left_elbow, y_left_elbow = int(left_elbow_landmark.x * frame_resized.shape[1]), int(left_elbow_landmark.y * frame_resized.shape[0])
                x_left_wrist, y_left_wrist = int(left_wrist_landmark.x * frame_resized.shape[1]), int(left_wrist_landmark.y * frame_resized.shape[0])
                x_left_hip, y_left_hip = int(left_hip_landmark.x * frame_resized.shape[1]),int(left_hip_landmark.y * frame_resized.shape[0])
                x_left_knee, y_left_knee = int(left_knee_landmark.x * frame_resized.shape[1]),int(left_knee_landmark.y * frame_resized.shape[0])
                x_left_ankle, y_left_ankle = int(left_ankle_landmark.x * frame_resized.shape[1]),int(left_ankle_landmark.y * frame_resized.shape[0])
                x_left_heel, y_left_heel = int(left_heel_landmark.x * frame_resized.shape[1]),int(left_heel_landmark.y * frame_resized.shape[0])
                x_left_foot, y_left_foot = int(left_foot_index_landmark.x * frame_resized.shape[1]),int(left_foot_index_landmark.y * frame_resized.shape[0])
                
                # Dibujar un círculos left body
                cv2.circle(frame_resized, (x_left_shoulder, y_left_shoulder), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_elbow, y_left_elbow), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_wrist, y_left_wrist), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_hip, y_left_hip), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_knee, y_left_knee), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_ankle, y_left_ankle), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_heel, y_left_heel), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_left_foot, y_left_foot), 5, (0,0,0), -1)
                
                #Obtencion de nodos lado derecho
                #numbers of landmarks 12,14,16,24,26,28,32,30
                right_shoulder_landmark = results_skeleto.pose_landmarks.landmark[12]
                right_elbow_landmark = results_skeleto.pose_landmarks.landmark[14]
                right_wrist_landmark = results_skeleto.pose_landmarks.landmark[16]
                right_hip_landmark = results_skeleto.pose_landmarks.landmark[24]
                right_knee_landmark = results_skeleto.pose_landmarks.landmark[26]
                right_ankle_landmark = results_skeleto.pose_landmarks.landmark[28]
                right_heel_landmark = results_skeleto.pose_landmarks.landmark[30]
                right_foot_index_landmark = results_skeleto.pose_landmarks.landmark[32]
                
                # X and y, Right body 
                x_right_shoulder, y_right_shoulder = int(right_shoulder_landmark.x * frame_resized.shape[1]), int(right_shoulder_landmark.y * frame_resized.shape[0])
                x_right_elbow, y_right_elbow = int(right_elbow_landmark.x *frame_resized.shape[1]), int(right_elbow_landmark.y * frame_resized.shape[0])
                x_right_wrist, y_right_wrist = int(right_wrist_landmark.x * frame_resized.shape[1]), int(right_wrist_landmark.y * frame_resized.shape[0])
                x_right_hip, y_right_hip = int(right_hip_landmark.x * frame_resized.shape[1]), int(right_hip_landmark.y * frame_resized.shape[0])
                x_right_knee, y_right_knee = int(right_knee_landmark.x * frame_resized.shape[1]), int(right_knee_landmark.y * frame_resized.shape[0])
                x_right_ankle, y_right_ankle = int(right_ankle_landmark.x * frame_resized.shape[1]), int(right_ankle_landmark.y * frame_resized.shape[0])
                x_right_heel, y_right_heel = int(right_heel_landmark.x * frame_resized.shape[1]), int(right_heel_landmark.y * frame_resized.shape[0])
                x_right_foot_index, y_right_foot_index = int(right_foot_index_landmark.x * frame_resized.shape[1]), int(right_foot_index_landmark.y * frame_resized.shape[0])
                
                # Dibujar un círculos right body
                cv2.circle(frame_resized, (x_right_shoulder, y_right_shoulder), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_elbow, y_right_elbow), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_wrist, y_right_wrist), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_hip, y_right_hip), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_knee, y_right_knee), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_ankle, y_right_ankle), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_heel, y_right_heel), 5, (0,0,0), -1)
                cv2.circle(frame_resized, (x_right_foot_index, y_right_foot_index), 5, (0,0,0), -1)
                
                # Conexiones hombros 
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_left_shoulder, y_left_shoulder), (0,0,0), 2)
                # Conexiones caderas
                cv2.line(frame_resized, (x_left_hip, y_left_hip),(x_right_hip, y_right_hip), (0,0,0), 2)
                # conexiones torso
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_hip, y_right_hip), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder),(x_left_hip, y_left_hip), (0,0,0), 2)
                # Conexiones left arm 
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder), (x_left_elbow, y_left_elbow), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_elbow, y_left_elbow), (x_left_wrist, y_left_wrist), (0,0,0), 2) 
                # Conexiones right arm
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_elbow, y_right_elbow), (0,0,0), 2) 
                cv2.line(frame_resized, (x_right_elbow, y_right_elbow), (x_right_wrist, y_right_wrist), (0,0,0), 2)
                # Conexiones left leg 
                cv2.line(frame_resized, (x_left_hip, y_left_hip),(x_left_knee, y_left_knee), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_knee, y_left_knee), (x_left_ankle, y_left_ankle), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_ankle, y_left_ankle), (x_left_heel, y_left_heel), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_heel, y_left_heel), (x_left_foot, y_left_foot), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_foot,y_left_foot), (x_left_ankle, y_left_ankle), (0,0,0), 2)
                
                # Conexiones right leg 
                cv2.line(frame_resized, (x_right_hip, y_right_hip),(x_right_knee, y_right_knee), (0,0,0), 2)
                cv2.line(frame_resized, (x_right_knee, y_right_knee), (x_right_ankle, y_right_ankle), (0,0,0), 2)
                cv2.line(frame_resized, (x_right_ankle, y_right_ankle), (x_right_heel, y_right_heel), (0,0,0), 2)
                cv2.line(frame_resized, (x_right_heel, y_right_heel), (x_right_foot_index, y_right_foot_index), (0,0,0), 2)
                cv2.line(frame_resized, (x_right_foot_index, y_right_foot_index), (x_right_ankle, y_right_ankle), (0,0,0), 2)
                
                #Obtencion de z mediante susweb  entreganto x e y de punto seleccionado 
                #print (f"right_shoulder x: {x_right_shoulder} Y: {y_right_shoulder} z: {sus.position_frame(x_right_shoulder, y_right_shoulder)}")
                
        return frame_resized
    
    def show_frame(self):
        if self.frame is not None:
            processed_frame = self.process_pose(self.frame)
            cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)

def Bodytracking(Mode):
    src = Mode
    threaded_camera = ThreadedCamera(src)
    while True:
        try:
            threaded_camera.show_frame()
        except AttributeError:
            pass
