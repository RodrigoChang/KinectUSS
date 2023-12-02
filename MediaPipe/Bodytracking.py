# Version 3.0
#Distance detecion hands 2.7 mt
import cv2
import mediapipe as mp
from threading import Thread
import time
import zmq
import numpy as np
import base64
from math import acos, degrees

mp_pose = mp.solutions.pose
class ThreadedCamera(object):
    def __init__(self,mode =0):
        try:
            self.capture = mode
        except ValueError:
            pass    
        try:
            self.ip = mode
        except ValueError:
            pass    
        self.frame_cam = None
        #Fotogramas a procesar
        self.FPS = 1 / 100
        # fotogramas tiempo en milisegundos 
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        #Parametros body
        confianza = 0.9
        self.pose =  mp_pose.Pose(static_image_mode=False, min_detection_confidence=confianza,min_tracking_confidence=confianza,model_complexity=2, smooth_landmarks= True)
    
    def update(self):
        try:
            context = zmq.Context()
            socket = context.socket(zmq.SUB)
            #5555 para recibir gabo
            #3001 para recibir mi pc
            socket.connect(f"tcp://{self.ip}:3001")
            socket.setsockopt_string(zmq.SUBSCRIBE, "")
            socket_active = True
        except Exception as e:
            socket_active = False
            print("No pude manito mio")
            print(f"Excepción durante la configuración del socket: {e}")
            # Puedes agregar más acciones aquí si es necesario
            pass
                    
        while True:
            #zqm connection
            if socket_active == True:
                #gabo
                """message = socket.recv()
                frame_data = np.frombuffer(message, dtype=np.uint8)
                frame = cv2.imdecode(frame_data, cv2.IMREAD_COLOR)
                frame = cv2.resize(frame, (512, 424))"""
                #mi pc
                msg = socket.recv()
                img = base64.b64decode(msg)
                npimg = np.frombuffer(img, dtype=np.uint8)
                frame = cv2.imdecode(npimg, 1)
                self.frame_cam = frame
                time.sleep(self.FPS)
            #Video conection    
            else:
                try:    
                    if self.capture is not None and self.capture.isOpened():
                        (self.status, self.frame_cam) = self.capture.read()
                    time.sleep(self.FPS)
                except Exception as e: 
                    print(f"Esta dando este error {e}")
                    break
    # Nico aqui esta la funcion del cuerpo 
    def process_pose(self, frame_cam):
        #frame_resized = cv2.resize(frame, resolution)
        frame_resized = frame_cam
        frame_rgb = cv2.cvtColor(frame_cam, cv2.COLOR_BGR2RGB)
        results_skeleto = self.pose.process(frame_rgb)
        #listaPosicion = [28, 27, 26, 25, 24, 23, 16, 15, 14, 13, 12, 11]
        #sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        """serverAddressPort = ("10.171.18.167", 5052)
        results = results_skeleto.process(frame_rgb)
        data = []"""
        
        if results_skeleto.pose_landmarks is not None:

                """ for i in listaPosicion:
                    new_data = []
                    new_data.append(int(results.pose_landmarks.landmark[i].x * 512))
                    new_data.append(int(results.pose_landmarks.landmark[i].y * 424))
                    new_data.append(round(float(results.pose_landmarks.landmark[i].z), 2))
                    data.extend([new_data[0],new_data[1], new_data[2]])
                    
            
                sock.sendto(str.encode(str(data)), serverAddressPort)"""
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
                """for name, landmark in zip(["left_shoulder", "left_elbow", "left_wrist", "left_hip", "left_knee", "left_ankle", "left_heel", "left_foot_index",
                                       "right_shoulder", "right_elbow", "right_wrist", "right_hip", "right_knee", "right_ankle", "right_heel", "right_foot_index"],
                                      [left_shoulder_landmark, left_elbow_landmark, left_wrist_landmark,
                                       left_hip_landmark, left_knee_landmark, left_ankle_landmark,
                                       left_heel_landmark, left_foot_index_landmark,
                                       right_shoulder_landmark, right_elbow_landmark, right_wrist_landmark,
                                       right_hip_landmark, right_knee_landmark, right_ankle_landmark,
                                       right_heel_landmark, right_foot_index_landmark]):
                    x, y, z = landmark.x, landmark.y, landmark.z"""
                    
                    
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
    while True:
        try:
            threaded_camera.show_frame(socket)
        except AttributeError:
            pass
