# Version 2.0
import cv2
import mediapipe as mp
from threading import Thread
import time
import tkinter as tk
from tkinter import Button

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose
mp_hands = mp.solutions.hands
threaded_camera = None

class ThreadedCamera(object):
    def __init__(self, src=0):
        #self.capture = cv2.VideoCapture(src, cv2.CAP_V4L)
        self.capture = cv2.VideoCapture(src)
        self.FPS = 1/100
        self.FPS_MS = int(self.FPS * 1000)
        self.pose = mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.90)
        self.hands = mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.7)
        self.up = False
        self.down = False
        self.count = 0
        self.running = True
        self.frame = None
        self.root = None
        self.thread = Thread(target=self.update, args=())
        self.thread.daemon = True
        self.thread.start()

    def update(self):
        while self.running:
            if self.capture is not None and self.capture.isOpened():
                (self.status, self.frame) = self.capture.read()
                self.show_frame()
            time.sleep(self.FPS)
    def stop(self):
        self.running = False
        self.thread.join()  # Esperar a que el hilo termine
        self.capture.release()
        self.root.quit()
        
    def process_pose(self, frame):
        frame_resized = cv2.resize(frame, (800, 600))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results_skeleto = self.pose.process(frame_rgb)
        
        if results_skeleto.pose_landmarks is not None:
            if results_skeleto.pose_landmarks is not None:
                #left body impar numbers
                #numbers of landmarks 11,13,15,23,25,27,29,31
                left_shoulder_landmark = results_skeleto.pose_landmarks.landmark[11]
                left_elbow_landmark = results_skeleto.pose_landmarks.landmark[13]
                left_wrist_landmark = results_skeleto.pose_landmarks.landmark[15]
                left_hip_landmark = results_skeleto.pose_landmarks.landmark[23]
                left_knee_landmark = results_skeleto.pose_landmarks.landmark[25]
                left_ankle_landmark = results_skeleto.pose_landmarks.landmark[27]
                left_heel_landmark = results_skeleto.pose_landmarks.landmark[29]
                left_foot_index_landmark = results_skeleto.pose_landmarks.landmark[31]
                
                # X and y left body 
                x_left_shoulder, y_left_shoulder = int(left_shoulder_landmark.x * frame_resized.shape[1]), int(left_shoulder_landmark.y * frame_resized.shape[0])
                x_left_elbow, y_left_elbow = int(left_elbow_landmark.x * frame_resized.shape[1]), int(left_elbow_landmark.y * frame_resized.shape[0])
                x_left_wrist, y_left_wrist = int(left_wrist_landmark.x * frame_resized.shape[1]), int(left_wrist_landmark.y * frame_resized.shape[0])
                x_left_hip, y_left_hip = int(left_hip_landmark.x * frame_resized.shape[1]),int(left_hip_landmark.y * frame_resized.shape[0])
                x_left_knee, y_left_knee = int(left_knee_landmark.x * frame_resized.shape[1]),int(left_knee_landmark.y * frame_resized.shape[0])
                x_left_ankle, y_left_ankle = int(left_ankle_landmark.x * frame_resized.shape[1]),int(left_ankle_landmark.y * frame_resized.shape[0])
                x_left_heel, y_left_heel = int(left_heel_landmark.x * frame_resized.shape[1]),int(left_heel_landmark.y * frame_resized.shape[0])
                x_left_foot, y_left_foot = int(left_foot_index_landmark.x * frame_resized.shape[1]),int(left_foot_index_landmark.y * frame_resized.shape[0])
                
                # Dibujar un círculos left body
                cv2.circle(frame_resized, (x_left_shoulder, y_left_shoulder), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_elbow, y_left_elbow), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_wrist, y_left_wrist), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_hip, y_left_hip), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_knee, y_left_knee), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_ankle, y_left_ankle), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_heel, y_left_heel), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_left_foot, y_left_foot), 5, (128, 0, 250), -1)
                #right body par numbers
                #numbers of landmarks 12,14,16,24,26,28,32,30
                right_shoulder_landmark = results_skeleto.pose_landmarks.landmark[12]
                right_elbow_landmark = results_skeleto.pose_landmarks.landmark[14]
                right_wrist_landmark = results_skeleto.pose_landmarks.landmark[16]
                right_hip_landmark = results_skeleto.pose_landmarks.landmark[24]
                right_knee_landmark = results_skeleto.pose_landmarks.landmark[26]
                right_ankle_landmark = results_skeleto.pose_landmarks.landmark[28]
                right_heel_landmark = results_skeleto.pose_landmarks.landmark[32]
                right_foot_index_landmark = results_skeleto.pose_landmarks.landmark[30]
                
                # X and y right body 
                x_right_shoulder, y_right_shoulder = int(right_shoulder_landmark.x * frame_resized.shape[1]), int(right_shoulder_landmark.y * frame_resized.shape[0])
                x_right_elbow, y_right_elbow = int(right_elbow_landmark.x *frame_resized.shape[1]), int(right_elbow_landmark.y * frame_resized.shape[0])
                x_right_wrist, y_right_wrist = int(right_wrist_landmark.x * frame_resized.shape[1]), int(right_wrist_landmark.y * frame_resized.shape[0])
                x_right_hip, y_right_hip = int(right_hip_landmark.x * frame_resized.shape[1]), int(right_hip_landmark.y * frame_resized.shape[0])
                x_right_knee, y_right_knee = int(right_knee_landmark.x * frame_resized.shape[1]), int(right_knee_landmark.y * frame_resized.shape[0])
                x_right_ankle, y_right_ankle = int(right_ankle_landmark.x * frame_resized.shape[1]), int(right_ankle_landmark.y * frame_resized.shape[0])
                x_right_heel, y_right_heel = int(right_heel_landmark.x * frame_resized.shape[1]), int(right_heel_landmark.y * frame_resized.shape[0])
                x_right_foot_index, y_right_foot_index = int(right_foot_index_landmark.x * frame_resized.shape[1]), int(right_foot_index_landmark.y * frame_resized.shape[0])
                


                # Dibujar un círculos right body
                cv2.circle(frame_resized, (x_right_shoulder, y_right_shoulder), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_elbow, y_right_elbow), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_wrist, y_right_wrist), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_hip, y_right_hip), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_knee, y_right_knee), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_ankle, y_right_ankle), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_heel, y_right_heel), 5, (128, 0, 250), -1)
                cv2.circle(frame_resized, (x_right_foot_index, y_right_foot_index), 5, (128, 0, 250), -1)
                

                # Conexiones hombros 
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_left_shoulder, y_left_shoulder), (255, 255, 255), 2)
                # Conexiones caderas
                cv2.line(frame_resized, (x_left_hip, y_left_hip),(x_right_hip, y_right_hip), (255,255,255), 2)
                # conexiones torso
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_hip, y_right_hip), (255, 255, 255), 2)
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder),(x_left_hip, y_left_hip), (255,255,255), 2)
                # Conexiones left arm 
                cv2.line(frame_resized, (x_left_shoulder, y_left_shoulder), (x_left_elbow, y_left_elbow), (255, 255, 255), 2)
                cv2.line(frame_resized, (x_left_elbow, y_left_elbow), (x_left_wrist, y_left_wrist), (255, 255, 255), 2) 
                # Conexiones right arm
                cv2.line(frame_resized, (x_right_shoulder, y_right_shoulder), (x_right_elbow, y_right_elbow), (255, 255,255), 2) 
                cv2.line(frame_resized, (x_right_elbow, y_left_elbow), (x_right_wrist, y_right_wrist), (255, 255,255), 2)
                # Conexiones left leg 
                cv2.line(frame_resized, (x_left_hip, y_left_hip),(x_left_knee, y_left_knee), (255,255,255), 2)
                cv2.line(frame_resized, (x_left_knee, y_left_knee), (x_left_ankle, y_left_ankle), (255,255,255), 2)
                cv2.line(frame_resized, (x_left_ankle, y_left_ankle), (x_left_heel, y_left_heel), (255,255,255), 2)
                cv2.line(frame_resized, (x_left_heel, y_left_heel), (x_left_foot, y_left_foot), (255,255,255), 2)
                # Conexiones right leg 
                cv2.line(frame_resized, (x_right_hip, y_right_hip),(x_right_knee, y_right_knee), (255,255,255), 2)
                cv2.line(frame_resized, (x_right_knee, y_right_knee), (x_right_ankle, y_right_ankle), (255,255,255), 2)
                cv2.line(frame_resized, (x_right_ankle, y_right_ankle), (x_right_heel, y_right_heel), (255,255,255), 2)
                cv2.line(frame_resized, (x_right_heel, y_right_heel), (x_right_foot_index, y_right_foot_index), (255,255,255), 2)
                # Calculo de angulos right arm and legs 
                
        return frame_resized

    def process_hands(self, frame):
        #frame_resized = cv2.resize(frame, (800, 600))
        frame_resized = cv2.resize(frame, (512, 424))
        frame_rgb = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2RGB)
        results = self.hands.process(frame_rgb)

        if results.multi_hand_landmarks:
            for landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    frame_resized, landmarks, mp_hands.HAND_CONNECTIONS,
                    mp_drawing.DrawingSpec(color=(128, 0, 250), thickness=2),
                    mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=2))
                
        return frame_resized

    def show_frame(self):
        if self.frame is not None:
            processed_frame = self.process_pose(self.frame)
            processed_frame = self.process_hands(processed_frame)
            cv2.imshow('frame', processed_frame)
            cv2.waitKey(self.FPS_MS)


def start_video_camera():
    global threaded_camera
    src = 0
    threaded_camera = ThreadedCamera(src)
    threaded_camera.root = root

def start_streaming():
    global threaded_camera
    src = 'udp://0.0.0.0:6000?overrun_nonfatal=1'
    threaded_camera = ThreadedCamera(src)
    threaded_camera.root = root


# Funciones para detener la captura
def stop_video():
    global threaded_camera
    if threaded_camera is not None:
        threaded_camera.stop()

def stop_streaming():
    global threaded_camera
    if threaded_camera is not None:
        threaded_camera.stop()

# Función para cerrar la aplicación
def quit_app():
    root.destroy()

# Crear la interfaz gráfica
root = tk.Tk()
root.title("Pose Detection App")

# Botón para la cámara de video
video_camera_button = tk.Button(root, text="Video Camera", command=start_video_camera)
video_camera_button.pack(pady=10)

# Botón para detener la cámara de video
stop_video_button = tk.Button(root, text="Stop Video", command=stop_video)
stop_video_button.pack(pady=10)

# Botón para el streaming
streaming_button = tk.Button(root, text="Streaming", command=start_streaming)
streaming_button.pack(pady=10)

# Botón para detener el streaming
stop_streaming_button = tk.Button(root, text="Stop Streaming", command=stop_streaming)
stop_streaming_button.pack(pady=10)

# Botón para salir
quit_button = tk.Button(root, text="Quit", command=quit_app)
quit_button.pack(pady=10)

# Iniciar el bucle principal de la interfaz gráfica
root.mainloop()