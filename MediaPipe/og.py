#Contador de SENTADILLAS / SQUATS 🦵| Python - MediaPipe - OpenCV
import cv2
import mediapipe as mp
import numpy as np
from math import acos, degrees

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

#cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap = cv2.VideoCapture("video_01.mp4")

up = False
dow = False
count = 0


with mp_pose.Pose(
    static_image_mode = False,
    min_detection_confidence=0.90) as pose:

    while True:
        ret, frame = cap.read()
        if ret == False:
            break

        frame = cv2.flip(frame, 1)
        height, width, _ = frame.shape
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = pose.process(frame_rgb)

        if results.pose_landmarks is not None:
           
            #Obtenemos las cordenas de los puntos deseados 24, 26, 28
            x1 = int(results.pose_landmarks.landmark[24].x * width)
            y1 = int(results.pose_landmarks.landmark[24].y * height)

            x2 = int(results.pose_landmarks.landmark[26].x * width)
            y2 = int(results.pose_landmarks.landmark[26].y * height)

            x3 = int(results.pose_landmarks.landmark[28].x * width)
            y3 = int(results.pose_landmarks.landmark[28].y * height)

            #Se aplica el teorema del coseno
            p1 = np.array([x1, y1])
            p2 = np.array([x2, y2])            
            p3 = np.array([x3, y3])

            l1 = np.linalg.norm(p2 - p3)
            l2 = np.linalg.norm(p1 - p3)
            l3 = np.linalg.norm(p1 - p2)

            #Calcular el ángulo
            angle = degrees(acos((l1*2 + l32 - l2*2) / (2 * l1 * l3)))

            if angle >= 160:
                up = True
            if up == True and dow == False and angle <= 70:
                dow = True
            if up == True and dow == True and angle >= 160:
                count += 1
                up = False
                dow = False

            #listaAngle.append(angle)
            #Visualizacion
            aux_image = np.zeros(frame.shape, np.uint8)
            cv2.line(aux_image, (x1, y1), (x2,y2), (255, 255, 0), 20)
            cv2.line(aux_image, (x2, y2), (x3,y3), (255, 255, 0), 20)
            cv2.line(aux_image, (x1, y1), (x3,y3), (255, 255, 0), 5)
            contours  =np.array([[x1, y1], [x2, y2], [x3 , y3]])
            cv2.fillPoly(aux_image, pts=[contours], color=(128, 0, 250 ))

            output = cv2.addWeighted(frame, 1, aux_image, 0.8, 0)

            #Se dibujan los puntos con las coordenas extraidas    
            cv2.circle(output, (x1, y1), 6, (0, 255, 255), 4)
            cv2.circle(output, (x2, y2), 6, (128, 0, 250), 4)
            cv2.circle(output, (x3, y3), 6, (255, 191, 0), 4)

            #Unimos la etiqueta en el frame o en el output
            cv2.putText(output, str(int(angle)), (x2 + 30, y2), 1, 1.5, (128, 0, 250), 2)
            cv2.putText(output, str(count), (10, 50), 1, 3.5, (128, 0, 250,), 2)
            cv2.imshow("output", output)
        cv2.imshow("Frame", frame)
        if cv2.waitKey(1) & 0XFF == 27:
            break

cap.release()
cv2.destroyAllWindows()

#nombre_archivo = 'Registro.csv'
#escribir_csv(nombre_archivo, listaAngle)
#print(f'Se ha creado el archivo "{nombre_archivo}" y se han escrito los datos.')