# Version 3.0
#Distance detecion hands 2.7 mt
#Libreria para controlar camaras y visualizacion
import cv2
#Libreria de google para obtener modelos de inteligencia artificial basada en tensorflow 
import mediapipe as mp
#Modulo de python para asignar tareas a Nucleos de procesador
from multiprocessing import Process, cpu_count
from threading import Thread
import time
import zmq
import numpy as np
import base64
from math import acos, degrees
import sys
sys.path.append('/home/fabian/Documents/Python codes/kinect/KinectUSS/KinectUSS/kinectia')
mp_pose = mp.solutions.pose
class ThreadedCamera(object):
    def __init__(self,mode =0):   
        try:
            print("activando streaming")
            self.ip = mode
        except ValueError:
            print(f"Ocurrio el siguiente error en def__init__:{ValueError}")
            pass    
        self.frame_cam = None
        #Fotogramas a procesar
        self.FPS = 1 / 60
        # fotogramas tiempo en milisegundos 
        self.FPS_MS = int(self.FPS * 1000)
    # Inicializar procesos y hilos
        #Hilos
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        #Procesos
        self.process_pose_Process = Process(target=self.process_pose, args=())
        self.process_pose_Process.daemon = True
        self.process_pose_Process.start()        
        #Parametros body
        confianza = 0.9
        self.pose =  mp_pose.Pose(static_image_mode=False, min_detection_confidence=confianza,min_tracking_confidence=confianza,model_complexity=2, smooth_landmarks= True)
    def update(self):
        try:
            print("Conectando...")
            context = zmq.Context()
            socket = context.socket(zmq.SUB)
            socket.connect(f"tcp://{self.ip}")
            socket.setsockopt(zmq.SUBSCRIBE, b'')
            
            self.socket_z = context.socket(zmq.REQ)
            IP= self.ip.split(":")
            print(IP[0])
            self.socket_z.connect(f'tcp://{IP[0]}:5557')
            
        except Exception as e:
            print(f"Excepción durante la configuración del socket: {e}")
        print("Conectado")
        while True:
            #zqm connection
            if self.ip != "0.0.0.0:3001":
                try:
                    #gabo
                    message = socket.recv()
                    frame_data = np.frombuffer(message, dtype=np.uint8)
                    frame = cv2.imdecode(frame_data, cv2.IMREAD_COLOR)
                    self.frame_cam = cv2.resize(frame, (512, 424))   
                except Exception as e:
                    print(f"Esta dando este error {e}")
                    self.thread._stop()
                    self.process_pose_Process.terminate()
                    break
            #Video conection zqm    
            else:
                try:    
                    #mi pc
                    msg = socket.recv()
                    img = base64.b64decode(msg)
                    npimg = np.frombuffer(img, dtype=np.uint8)
                    frame = cv2.imdecode(npimg, 1)
                    self.frame_cam = frame
                    time.sleep(self.FPS)
                except Exception as e: 
                    print(f"Esta dando este error {e}")
                    break
    #Funcion_para crear eskeleto 
    def process_pose(self, frame_cam):
        frame_resized = cv2.resize(frame_cam, (512, 424))
        frame_resized = frame_cam
        frame_rgb = cv2.cvtColor(frame_cam, cv2.COLOR_BGR2RGB)
        results_skeleto = self.pose.process(frame_rgb)
        if results_skeleto.pose_landmarks is not None:                
                # Definir los índices de los landmarks para el lado izquierdo y derecho del cuerpo
                left_landmark_indices = [11, 13, 15, 23, 25, 27, 29, 31]
                right_landmark_indices = [12, 14, 16, 24, 26, 28, 30, 32]
                # Inicializar listas para almacenar las coordenadas x e y de cada landmark
                left_x_coords, left_y_coords = [], []
                right_x_coords, right_y_coords = [], []
                # Iterar sobre los índices y obtener las coordenadas x e y de cada landmark para el lado izquierdo
                for index in left_landmark_indices:
                    landmark = results_skeleto.pose_landmarks.landmark[index]
                    left_x_coords.append(int(landmark.x * frame_resized.shape[1]))
                    left_y_coords.append(int(landmark.y * frame_resized.shape[0]))
                # Iterar sobre los índices y obtener las coordenadas x e y de cada landmark para el lado derecho
                for index in right_landmark_indices:
                    landmark = results_skeleto.pose_landmarks.landmark[index]
                    right_x_coords.append(int(landmark.x * frame_resized.shape[1]))
                    right_y_coords.append(int(landmark.y * frame_resized.shape[0]))
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
                def ask_z(message):
                    self.socket_z.send(message)
                    response_msg = self.socket_z.recv()
                    print(f"Received response: {response_msg.decode('utf-8')}")
                try:
                    message = f"{left_x_coords[0]},{left_y_coords[0]}".encode('utf-8')
                    ask_z(message)
                except Exception as e:
                    print(f"{e}")
                    print(f"hombro izquierdo{left_x_coords[0],left_y_coords[0]}")
        return frame_resized


    def show_frame(self,socket):
        if self.frame_cam is not None:
            processed_frame = self.process_pose(self.frame_cam)
            _, img_encoded = cv2.imencode('.jpg', processed_frame)
            msg = base64.b64encode(img_encoded.tobytes())
            socket.send(msg)
            #cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)


def Bodytracking(mode):
    context = zmq.Context()
    socket = context.socket(zmq.PUB)
    socket.bind("tcp://0.0.0.0:3002")
    threaded_camera = ThreadedCamera(mode)
    print(cpu_count())
    while True:
        try:
            threaded_camera.show_frame(socket)
            
        except AttributeError:
            print(f"A Ocurrido un error")
            pass
