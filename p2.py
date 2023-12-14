# Version 3.0

#Distance detecion hands 2.7 mt
import cv2
import mediapipe as mp
from threading import Thread
import time
#import susweb as sus
import numpy as np
from math import acos, degrees

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
resolution =(800,600)

class ThreadedCamera(object):
    def __init__(self, src=0):
        #Capturar por camara ,descomentar para usar camara.., comentar para usar streaming 
        self.capture = cv2.VideoCapture(src)
        #Capturar camara Kinect via streaming, descomentar para usar streaming,comentar para usar camara, 
        #self.capture = cv2.VideoCapture(src)
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()
        #Parametros body
        #model_complexity=1 default model_complexity=2 da error investiga esto Carlos  
        self.pose =  mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.9,min_tracking_confidence=0.9,model_complexity=1, smooth_landmarks= False)
        #Parametros Hands
        self.hands = mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.9,min_tracking_confidence=0.9,model_complexity=1)

    def update(self):
        while True:
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
            time.sleep(self.FPS)
    
    def filter_arm_keypoints(self, results, side):
        if results.pose_landmarks:
            if side == 'right':
                arm_keypoints = [mp.solutions.pose.PoseLandmark.RIGHT_SHOULDER, mp.solutions.pose.PoseLandmark.RIGHT_ELBOW, mp.solutions.pose.PoseLandmark.RIGHT_WRIST]
            elif side == 'left':
                arm_keypoints = [mp.solutions.pose.PoseLandmark.LEFT_SHOULDER, mp.solutions.pose.PoseLandmark.LEFT_ELBOW, mp.solutions.pose.PoseLandmark.LEFT_WRIST]

            filtered_arm_keypoints = [point for point in arm_keypoints if results.pose_landmarks.landmark[point].visibility > 0.5]
            return filtered_arm_keypoints
        return []
    # Nico aqui esta la funcion del cuerpo 
    def process_pose(self, frame):
        frame_resized = cv2.resize(frame, resolution)
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results_skeleto = self.pose.process(frame_rgb)
            
        if results_skeleto.pose_landmarks is not None:
                """for side in ['left', 'right']:
                # Filtrar puntos clave del brazo
                filtered_arm_keypoints = self.filter_arm_keypoints(results_skeleto, side=side)"""

                # Dibujar el esqueleto con los puntos clave del brazo filtrados
                """if filtered_arm_keypoints:
                    for landmark in filtered_arm_keypoints:
                        point = results_skeleto.pose_landmarks.landmark[landmark]
                        x, y = int(point.x * frame_resized.shape[1]), int(point.y * frame_resized.shape[0])
                        cv2.circle(frame_resized, (x, y), 5, (0, 0, 0), -1)"""
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
                x_left_hip, y_left_hip = int(left_hip_landmark.x * frame_resized.shape[1]), int(left_hip_landmark.y * frame_resized.shape[0])
                x_left_knee, y_left_knee = int(left_knee_landmark.x * frame_resized.shape[1]), int(left_knee_landmark.y * frame_resized.shape[0])
                x_left_ankle, y_left_ankle = int(left_ankle_landmark.x * frame_resized.shape[1]), int(left_ankle_landmark.y * frame_resized.shape[0])
                x_left_heel, y_left_heel = int(left_heel_landmark.x * frame_resized.shape[1]), int(left_heel_landmark.y * frame_resized.shape[0])
                x_left_foot, y_left_foot = int(left_foot_index_landmark.x * frame_resized.shape[1]), int(left_foot_index_landmark.y * frame_resized.shape[0])

                
               

                # Obtencion de nodos lado derecho
                # numbers of landmarks 12,14,16,24,26,28,32,30
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
                x_right_elbow, y_right_elbow = int(right_elbow_landmark.x * frame_resized.shape[1]), int(right_elbow_landmark.y * frame_resized.shape[0])
                x_right_wrist, y_right_wrist = int(right_wrist_landmark.x * frame_resized.shape[1]), int(right_wrist_landmark.y * frame_resized.shape[0])
                x_right_hip, y_right_hip = int(right_hip_landmark.x * frame_resized.shape[1]), int(right_hip_landmark.y * frame_resized.shape[0])
                x_right_knee, y_right_knee = int(right_knee_landmark.x * frame_resized.shape[1]), int(right_knee_landmark.y * frame_resized.shape[0])
                x_right_ankle, y_right_ankle = int(right_ankle_landmark.x * frame_resized.shape[1]), int(right_ankle_landmark.y * frame_resized.shape[0])
                x_right_heel, y_right_heel = int(right_heel_landmark.x * frame_resized.shape[1]), int(right_heel_landmark.y * frame_resized.shape[0])
                x_right_foot_index, y_right_foot_index = int(right_foot_index_landmark.x * frame_resized.shape[1]), int(right_foot_index_landmark.y * frame_resized.shape[0])

                # Filtrar puntos clave del brazo izquierdo
                left_arm_keypoints = [results_skeleto.pose_landmarks.landmark[i] for i in [11, 13, 15, 23, 25, 27, 29, 31]]
                left_arm_keypoints_visible = [point for point in left_arm_keypoints if point.visibility > 0.5]

                # Dibujar círculos para puntos clave del brazo izquierdo
                for point in left_arm_keypoints_visible:
                   x, y = int(point.x * frame_resized.shape[1]), int(point.y * frame_resized.shape[0])
                   cv2.circle(frame_resized, (x, y), 5, (0, 0, 0), -1)

                # Filtrar puntos clave del brazo derecho
                right_arm_keypoints = [results_skeleto.pose_landmarks.landmark[i] for i in [12, 14, 16, 24, 26, 28, 30, 32]]
                right_arm_keypoints_visible = [point for point in right_arm_keypoints if point.visibility > 0.5]

                # Dibujar círculos para puntos clave del brazo derecho
                for point in right_arm_keypoints_visible:
                   x, y = int(point.x * frame_resized.shape[1]), int(point.y * frame_resized.shape[0])
                   cv2.circle(frame_resized, (x, y), 5, (0, 0, 0), -1)

                # Conexiones hombros
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_left_shoulder, y_left_shoulder), (0, 0, 0), 2)
                # Conexiones caderas
                cv2.line(frame_resized, (x_left_hip, y_left_hip), (x_right_hip, y_right_hip), (0, 0, 0), 2)
                # conexiones torso
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_hip, y_right_hip), (0, 0, 0), 2)
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder), (x_left_hip, y_left_hip), (0, 0, 0), 2)
                
                
                # Conexiones left arm
                for i in range(len(left_arm_keypoints_visible) - 2):
                   point_a = left_arm_keypoints_visible[i]
                   point_b = left_arm_keypoints_visible[i + 1]
                   x1, y1 = int(point_a.x * frame_resized.shape[1]), int(point_a.y * frame_resized.shape[0])
                   x2, y2 = int(point_b.x * frame_resized.shape[1]), int(point_b.y * frame_resized.shape[0])
                   cv2.line(frame_resized, (x1, y1), (x2, y2), (0, 0, 0), 2)

                # Conexiones right arm
                for i in range(len(right_arm_keypoints_visible) - 2):
                   point_a = right_arm_keypoints_visible[i]
                   point_b = right_arm_keypoints_visible[i + 1]
                   x1, y1 = int(point_a.x * frame_resized.shape[1]), int(point_a.y * frame_resized.shape[0])
                   x2, y2 = int(point_b.x * frame_resized.shape[1]), int(point_b.y * frame_resized.shape[0])
                   cv2.line(frame_resized, (x1, y1), (x2, y2), (0, 0, 0), 2)

                

                   
                   
                
                """# Conexiones left arm 
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder), (x_left_elbow, y_left_elbow), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_elbow, y_left_elbow), (x_left_wrist, y_left_wrist), (0,0,0), 2) 
                # Conexiones right arm
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_elbow, y_right_elbow), (0,0,0), 2) 
                cv2.line(frame_resized, (x_right_elbow, y_right_elbow), (x_right_wrist, y_right_wrist), (0,0,0), 2)"""
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
                
                """# Conexiones left arm 
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder), (x_left_elbow, y_left_elbow), (0,0,0), 2)
                cv2.line(frame_resized, (x_left_elbow, y_left_elbow), (x_left_wrist, y_left_wrist), (0,0,0), 2) 
                # Conexiones right arm
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_elbow, y_right_elbow), (0,0,0), 2) 
                cv2.line(frame_resized, (x_right_elbow, y_right_elbow), (x_right_wrist, y_right_wrist), (0,0,0), 2)"""
                
                
                """ #Listad de angulos
                Angle_leg_R =[[x_right_hip,y_right_hip],[x_right_knee,y_right_knee],[x_right_ankle,y_right_ankle]]
                Angle_leg_L =[[x_left_hip,y_left_hip],[x_left_knee,y_left_knee],[x_left_ankle,y_left_ankle]]
                Angle_foot_R=[[x_right_knee,y_right_knee],[x_right_ankle,y_right_ankle],[x_right_foot_index,y_right_foot_index]]
                Angle_foot_L=[[x_left_knee,y_left_knee],[x_left_ankle,y_left_ankle],[x_left_foot,y_left_foot]]
                Angle_arm_R=[[x_right_shoulder,y_right_shoulder],[x_right_elbow,y_right_elbow],[x_right_wrist,y_right_wrist]]
                Angle_arm_L=[[x_left_shoulder,y_left_shoulder],[x_left_elbow,y_left_elbow],[x_left_wrist,y_left_wrist]]
                Angle_incline_R=[[x_right_shoulder,y_right_shoulder],[x_right_hip,y_right_hip],[x_right_ankle,y_right_ankle]]
                Angle_incline_L=[[x_left_shoulder,y_left_shoulder],[x_left_hip,y_left_hip],[x_left_ankle,y_left_ankle]]
                
                # Calculo de angulos 
                for landmark in zip([Angle_leg_R,Angle_leg_L,Angle_foot_R,Angle_foot_L,Angle_arm_R,Angle_arm_L,Angle_incline_R,Angle_incline_L]):
                    for i in landmark:
                        p1 = np.array(i[0])
                        p2 = np.array(i[1])            
                        p3 = np.array(i[2]) 
                
                        l1 = np.linalg.norm(p2 - p3)
                        l2 = np.linalg.norm(p1 - p3)
                        l3 = np.linalg.norm(p1 - p2)
                        
                        #Calcular el ángulo utilizando arcoseno 
                        angle = degrees(acos((l1**2 + l3**2 - l2**2) / (2 * l1 * l3)))
                        #print( angle)
                        
                        #Visualizacion de lineas
                        x=0
                        y=1
                        cv2.line(frame_resized, (i[0][x],i[0][y]), (i[1][x],i[1][y]), (255, 255, 0), 5)
                        cv2.line(frame_resized, (i[1][x],i[1][y]), (i[2][x],i[2][y]), (255, 255, 0), 5)
                        cv2.line(frame_resized, (i[0][x],i[0][y]), (i[2][x],i[2][y]), (255, 255, 0), 5)

                        # agregar el texto sobre la linea 
                        text = str(int(angle))
                        text_position = ((i[1][0] + i[2][0]) // 2, min(i[1][1], i[2][1]) - 10)
                        cv2.putText(frame_resized, text, text_position, cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0,0,0), 1, cv2.LINE_AA)"""

                    #Visualizacion de datos 
                """for name, landmark in zip(["left_shoulder", "left_elbow", "left_wrist", "left_hip", "left_knee", "left_ankle", "left_heel", "left_foot_index",
                                       "right_shoulder", "right_elbow", "right_wrist", "right_hip", "right_knee", "right_ankle", "right_heel", "right_foot_index"],
                                      [left_shoulder_landmark, left_elbow_landmark, left_wrist_landmark,
                                       left_hip_landmark, left_knee_landmark, left_ankle_landmark,
                                       left_heel_landmark, left_foot_index_landmark,
                                       right_shoulder_landmark, right_elbow_landmark, right_wrist_landmark,
                                       right_hip_landmark, right_knee_landmark, right_ankle_landmark,
                                       right_heel_landmark, right_foot_index_landmark]):
                    x, y, z = landmark.x, landmark.y, landmark.z
                    cv2.putText(frame_resized, f"({x},{y},{z})", (int(x * frame_resized.shape[1]), int(y * frame_resized.shape[0])),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 0), 1, cv2.LINE_AA)"""
                #Obtencion de z mediante susweb  entreganto x e y de punto seleccionado 
                #print (f"right_shoulder x: {x_right_shoulder} Y: {y_right_shoulder} z: {sus.position_frame(x_right_shoulder, y_right_shoulder)}")
                
        return frame_resized
    
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

    """def filter_arm_keypoints(self, results, side='right'):
        if results.pose_landmarks:
            if side == 'right':
                arm_keypoints = [mp_pose.PoseLandmark.RIGHT_SHOULDER, mp_pose.PoseLandmark.RIGHT_ELBOW, mp_pose.PoseLandmark.RIGHT_WRIST]
            else:
                arm_keypoints = [mp_pose.PoseLandmark.LEFT_SHOULDER, mp_pose.PoseLandmark.LEFT_ELBOW, mp_pose.PoseLandmark.LEFT_WRIST]

            filtered_arm_keypoints = [point for point in arm_keypoints if results.pose_landmarks.landmark[point]   > 0.5]
            return filtered_arm_keypoints
        return []"""

    def show_frame(self):
        if self.frame is not None:
            processed_frame = self.process_pose(self.frame)
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