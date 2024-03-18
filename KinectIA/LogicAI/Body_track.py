#Version 4.0
#Librerias requeridas
#Imports
import cv2
import mediapipe as mp
import time
import zmq
import numpy as np
import base64
import sys
#From imports
from multiprocessing import Process, cpu_count
from threading import Thread
#from . import Csv_handler
from math import acos, degrees
from Apis import SRR

#Initial Variables
print("activando streaming")
mp_pose = mp.solutions.pose
FPS = 1 / 60
FPS_MS = int(FPS * 1000)
thread = Thread(target=update(), args=())
thread.daemon = True
thread.start()
process_pose_Process = Process(target=process_pose(), args=())
process_pose_Process.daemon = True
process_pose_Process.start()        
pose =  mp_pose.Pose(static_image_mode=False, min_detection_confidence= 0.9,min_tracking_confidence=confianza,model_complexity=2, smooth_landmarks= True)

#Open conections
Conect_to_z(ip)    
while True:
    #zqm connection
    frame_cam = Received_frame(ip)

#Funcion_para crear eskeleto 
def process_pose(frame_cam):
    frame_resized = frame_cam
    frame_rgb = cv2.cvtColor(frame_cam, cv2.COLOR_BGR2RGB)
    results_skeleto = self.pose.process(frame_rgb)
    if results_skeleto.pose_landmarks is not None:
            
            # Definir los índices de los landmarks para el lado izquierdo y derecho del cuerpo
            left_landmark_indices = [11, 13, 15, 23, 25, 27, 29, 31]
            right_landmark_indices = [12, 14, 16, 24, 26, 28, 30, 32]
            # Inicializar listas para almacenar las coordenadas x e y de cada landmark
            left_x_coords, left_y_coords,left_z_coords = [], [], []
            right_x_coords, right_y_coords,right_z_coords = [], [],[]
            
            # Iterar sobre los índices y obtener las coordenadas x e y de cada landmark para el lado izquierdo
            for index in left_landmark_indices:
                landmark = results_skeleto.pose_landmarks.landmark[index]
                left_x_coords.append(int(landmark.x * frame_resized.shape[1]))
                left_y_coords.append(int(landmark.y * frame_resized.shape[0]))
                message =f"{int(landmark.x * frame_resized.shape[1])},{int(landmark.y * frame_resized.shape[0])}"
                left_z_coords.append(message)
            # Iterar sobre los índices y obtener las coordenadas x e y de cada landmark para el lado derecho
            for index in right_landmark_indices:
                landmark = results_skeleto.pose_landmarks.landmark[index]
                right_x_coords.append(int(landmark.x * frame_resized.shape[1]))
                right_y_coords.append(int(landmark.y * frame_resized.shape[0]))
                message =f"{int(landmark.x * frame_resized.shape[1])},{int(landmark.y * frame_resized.shape[0])}"
                right_z_coords.append(message)
            
            z = Thread(target=ask_z(), args=(left_z_coords, right_z_coords))
            z.start()
            
            # Dibujar círculos para cada landmark del lado izquierdo
            for x, y in zip(left_x_coords, left_y_coords):
                cv2.circle(frame_resized, (x, y), 5, (0, 0, 0), -1)
            # Dibujar círculos para cada landmark del lado derecho
            for x, y in zip(right_x_coords, right_y_coords):
                cv2.circle(frame_resized, (x, y), 5, (0, 0, 0), -1)
            # Conexiones hombros 
            cv2.line(frame_resized, (right_x_coords[0], right_y_coords[0]), (left_x_coords[0], left_y_coords[0]), (0,0,0), 2)
            # Conexiones caderas
            cv2.line(frame_resized, (left_x_coords[3], left_y_coords[3]), (right_x_coords[3], right_y_coords[3]), (0,0,0), 2)
            # Conexiones torso
            cv2.line(frame_resized, (right_x_coords[0], right_y_coords[0]), (right_x_coords[3], right_y_coords[3]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[0], left_y_coords[0]), (left_x_coords[3], left_y_coords[3]), (0,0,0), 2)
            # Conexiones left arm 
            cv2.line(frame_resized, (left_x_coords[0], left_y_coords[0]), (left_x_coords[1], left_y_coords[1]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[1], left_y_coords[1]), (left_x_coords[2], left_y_coords[2]), (0,0,0), 2) 
            # Conexiones right arm
            cv2.line(frame_resized, (right_x_coords[0], right_y_coords[0]), (right_x_coords[1], right_y_coords[1]), (0,0,0), 2) 
            cv2.line(frame_resized, (right_x_coords[1], right_y_coords[1]), (right_x_coords[2], right_y_coords[2]), (0,0,0), 2)
            # Conexiones left leg 
            cv2.line(frame_resized, (left_x_coords[3], left_y_coords[3]), (left_x_coords[4], left_y_coords[4]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[4], left_y_coords[4]), (left_x_coords[5], left_y_coords[5]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[5], left_y_coords[5]), (left_x_coords[6], left_y_coords[6]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[6], left_y_coords[6]), (left_x_coords[7], left_y_coords[7]), (0,0,0), 2)
            cv2.line(frame_resized, (left_x_coords[7], left_y_coords[7]), (left_x_coords[5], left_y_coords[5]), (0,0,0), 2)
            # Conexiones right leg 
            cv2.line(frame_resized, (right_x_coords[3], right_y_coords[3]), (right_x_coords[4], right_y_coords[4]), (0,0,0), 2)
            cv2.line(frame_resized, (right_x_coords[4], right_y_coords[4]), (right_x_coords[5], right_y_coords[5]), (0,0,0), 2)
            cv2.line(frame_resized, (right_x_coords[5], right_y_coords[5]), (right_x_coords[6], right_y_coords[6]), (0,0,0), 2)
            cv2.line(frame_resized, (right_x_coords[6], right_y_coords[6]), (right_x_coords[7], right_y_coords[7]), (0,0,0), 2)
            cv2.line(frame_resized, (right_x_coords[7], right_y_coords[7]), (right_x_coords[5], right_y_coords[5]), (0,0,0), 2)
            z.join()
    return frame_resized

def show_frame(socket):
        if  process_pose(frame_cam) is not None:
            processed_frame = process_pose(frame_cam)
            _, img_encoded = cv2.imencode('.jpg', processed_frame)
            msg = base64.b64encode(img_encoded.tobytes())
            socket.send(msg)
            #cv2.imshow('frame', processed_frame)
            cv2.waitKey(FPS_MS)

def Bodytracking(IP):
    Open_Frame_send()
    ip = IP
    #print(cpu_count())
    while True:
        try:
            show_frame(socket)
        except AttributeError:
            print(f"A Ocurrido un error")
            pass