#mediapipe==0.10.8
#numpy==1.21.5
#opencv==4.5.5.64

import cv2
import mediapipe as mp
import numpy as np
import csv
import time
import socket
from math import acos, degrees

mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
#cap = cv2.VideoCapture("video_01.mp4")
#cap = cv2.VideoCapture("video580.mp4")


rodillaDerecha = []
Data = [["Nombre", "N°", "Angulo", "x","y","z","Tiempo"]]

listaPosicion = [28, 27, 26, 25, 24, 23, 16, 15, 14, 13, 12, 11]
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverAddressPort = ("127.0.0.1", 5052)

with mp_pose.Pose(
    static_image_mode = False,
    model_complexity = 2,
    smooth_landmarks = True,  
    min_detection_confidence=0.95,
    min_tracking_confidence = 0.95) as pose:
    while True:
        ret, frame = cap.read()
        if ret == False:
            break

        frame = cv2.flip(frame, 1)
        height, width, _ = frame.shape
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = pose.process(frame_rgb)
        data = []

        if results.pose_landmarks is not None:
            
            for i in listaPosicion:
                new_data = []
                new_data.append(int(results.pose_landmarks.landmark[i].x * width))
                new_data.append(int(results.pose_landmarks.landmark[i].y * height))
                new_data.append(round(float(results.pose_landmarks.landmark[i].z), 2))
                data.extend([new_data[0],new_data[1], new_data[2]])
                
            
            sock.sendto(str.encode(str(data)), serverAddressPort)
            #Obtenemos las cordenas de los puntos deseados 24, 26, 28
            x1 = int(results.pose_landmarks.landmark[24].x * width)
            y1 = int(results.pose_landmarks.landmark[24].y * height)

            x2 = int(results.pose_landmarks.landmark[26].x * width)
            y2 = int(results.pose_landmarks.landmark[26].y * height)

            x3 = int(results.pose_landmarks.landmark[28].x * width)
            y3 = int(results.pose_landmarks.landmark[28].y * height)

            #Obtenemos las cordenas de los puntos deseados 23, 25, 27
            x4 = int(results.pose_landmarks.landmark[23].x * width)
            y4 = int(results.pose_landmarks.landmark[23].y * height)

            x5 = int(results.pose_landmarks.landmark[25].x * width)
            y5 = int(results.pose_landmarks.landmark[25].y * height)

            x6 = int(results.pose_landmarks.landmark[27].x * width)
            y6 = int(results.pose_landmarks.landmark[27].y * height)

            #Obtener las cordenas 11, 13, 15
            x7 = int(results.pose_landmarks.landmark[11].x * width)
            y7 = int(results.pose_landmarks.landmark[11].y * height)

            x8 = int(results.pose_landmarks.landmark[13].x * width)
            y8 = int(results.pose_landmarks.landmark[13].y * height)

            x9 = int(results.pose_landmarks.landmark[15].x * width)
            y9 = int(results.pose_landmarks.landmark[15].y * height)

            #Obtener las cordenas 12, 14, 16

            x10 = int(results.pose_landmarks.landmark[12].x * width)
            y10 = int(results.pose_landmarks.landmark[12].y * height)

            x11 = int(results.pose_landmarks.landmark[14].x * width)
            y11 = int(results.pose_landmarks.landmark[14].y * height)
            #se debe definir el z con un parametro del ambiente
            z11 = float(results.pose_landmarks.landmark[14].z)

            x12 = int(results.pose_landmarks.landmark[16].x * width)
            y12 = int(results.pose_landmarks.landmark[16].y * height)

            #Talon y punta del pie derecho
            x13 = int(results.pose_landmarks.landmark[30].x * width)
            y13 = int(results.pose_landmarks.landmark[30].y * height)

            x14 = int(results.pose_landmarks.landmark[32].x * width)
            y14 = int(results.pose_landmarks.landmark[32].y * height)

            #Talon y punta del pie derecho
            x15 = int(results.pose_landmarks.landmark[29].x * width)
            y15 = int(results.pose_landmarks.landmark[29].y * height)

            x16 = int(results.pose_landmarks.landmark[31].x * width)
            y16 = int(results.pose_landmarks.landmark[31].y * height)

            

            #Se define los puntos
            p1 = np.array([x1, y1]) #24
            p2 = np.array([x2, y2]) #26        
            p3 = np.array([x3, y3]) #28

            p4 = np.array([x4, y4])
            p5 = np.array([x5, y5])
            p6 = np.array([x6, y6])#27

            p7 = np.array([x7, y7]) #11
            p8 = np.array([x8, y8]) #13
            p9 = np.array([x9, y9]) #15

            p10 = np.array([x10, y10]) #12
            p11 = np.array([x11, y11]) #14
            p12 = np.array([x12, y12]) #16

            p13 = np.array([x13, y13]) #30
            p14 = np.array([x14, y14]) #32
            p15 = np.array([x15, y15]) #29
            p16 = np.array([x16, y16]) #31

            #Se define la linea
            l1 = np.linalg.norm(p2 - p3)
            l2 = np.linalg.norm(p1 - p3)
            l3 = np.linalg.norm(p1 - p2)

            l4 = np.linalg.norm(p5 - p6)
            l5 = np.linalg.norm(p4 - p6)
            l6 = np.linalg.norm(p4 - p5)

            l7 = np.linalg.norm(p8 - p9)#13 - 15
            l8 = np.linalg.norm(p7 - p9)#11 - 15
            l9 = np.linalg.norm(p7 - p8)#11 - 13

            l10 = np.linalg.norm(p11 - p12)#14 - 16
            l11 = np.linalg.norm(p10 - p12)#12 - 15
            l12 = np.linalg.norm(p10 - p11)#11 - 13

            l13 = np.linalg.norm(p13 - p3)#28 - 30
            l14 = np.linalg.norm(p13 - p14)#30 - 32
            l15 = np.linalg.norm(p14 - p3)#32 - 28


            #Calcular el ángulo rodilla izquirda
            angle1 = degrees(acos((l1**2 + l3**2 - l2**2) / (2 * l1 * l3)))

            #Calcular el ángulo rodilla derecha
            angle2 = degrees(acos((l4**2 + l6**2 - l5**2) / (2 * l4 * l6)))

            #Calcular el ángulo codo derecha
            angle3 = degrees(acos((l7**2 + l9**2 - l8**2) / (2 * l7 * l9)))

            #Calcular el ángulo codo derecha
            angle4 = degrees(acos((l10**2 + l12**2 - l11**2) / (2 * l10 * l12)))

            codoDerech = ["Rodilla Derecho", 14, angle2, x5, y5, 1,time.asctime()]
            Data.append(codoDerech)

            #Visualizacion y dibujo
            aux_image = np.zeros(frame.shape, np.uint8)
            cv2.line(aux_image, (x1, y1), (x2,y2), (255, 255, 0),5)
            cv2.line(aux_image, (x2, y2), (x3,y3), (255, 255, 0), 5)

            cv2.line(aux_image, (x4, y4), (x5,y5), (255, 255, 0), 5)
            cv2.line(aux_image, (x5, y5), (x6,y6), (255, 255, 0), 5)

            cv2.line(aux_image, (x7, y7), (x8,y8), (255, 255, 0), 5)
            cv2.line(aux_image, (x8, y8), (x9,y9), (255, 255, 0), 5)

            cv2.line(aux_image, (x10, y10), (x11,y11), (255, 255, 0), 5)
            cv2.line(aux_image, (x11, y11), (x12,y12), (255, 255, 0), 5)
#----------------------------------------------------------------------------------
            cv2.line(aux_image, (x7, y7), (x10,y10), (255, 255, 0), 5)
            cv2.line(aux_image, (x1, y1), (x4,y4), (255, 255, 0), 5)

            cv2.line(aux_image, (x10, y10), (x1,y1), (255, 255, 0), 5)
            cv2.line(aux_image, (x7, y7), (x4,y4), (255, 255, 0), 5)


            #Pie derecho
            cv2.line(aux_image, (x13, y13), (x3,y3), (255, 255, 0), 5)
            cv2.line(aux_image, (x14, y14), (x13, y13), (255, 255, 0), 5)
            cv2.line(aux_image, (x14, y14), (x3, y3), (255, 255, 0), 5)

            #Pie izquierdo
            cv2.line(aux_image, (x6, y6), (x15,y15), (255, 255, 0), 5)
            cv2.line(aux_image, (x16, y16), (x15, y15), (255, 255, 0), 5)
            cv2.line(aux_image, (x6, y6), (x16, y16), (255, 255, 0), 5)
            

            output = cv2.addWeighted(frame, 1, aux_image, 0.8, 0)
            

            #Se dibujan los puntos con las coordenas extraidas    
            cv2.circle(output, (x1, y1), 6, (0, 255, 255), 4)
            cv2.circle(output, (x2, y2), 6, (128, 0, 250), 4)
            cv2.circle(output, (x3, y3), 6, (255, 191, 0), 4)

            cv2.circle(output, (x4, y4), 6, (0, 255, 255), 4)
            cv2.circle(output, (x5, y5), 6, (128, 0, 250), 4)
            cv2.circle(output, (x6, y6), 6, (255, 191, 0), 4)

            cv2.circle(output, (x7, y7), 6, (0, 255, 255), 4)
            cv2.circle(output, (x8, y8), 6, (128, 0, 250), 4)
            cv2.circle(output, (x9, y9), 6, (255, 191, 0), 4)

            cv2.circle(output, (x10,y10), 6, (0, 255, 255), 4)
            cv2.circle(output, (x11,y11), 6, (128, 0, 250), 4)
            cv2.circle(output, (x12,y12), 6, (255, 191, 0), 4)

            cv2.circle(output, (x13,y13), 6, (255, 191, 0), 4)
            cv2.circle(output, (x14,y14), 6, (255, 191, 0), 4)
            
            cv2.circle(output, (x15,y15), 6, (255, 191, 0), 4)
            cv2.circle(output, (x16,y16), 6, (255, 191, 0), 4)


            #Unimos la etiqueta en el frame o en el output
            #Etiquetas para las rodillas
            cv2.putText(output, str(int(angle1)), (x2 + 30, y2), 1, 1.5, (128, 0, 250), 2)
            cv2.putText(output, str(int(angle2)), (x5 + 30, y5), 1, 1.5, (128, 0, 250), 2)
            #Etiquetas para los codos
            cv2.putText(output, str(int(angle3)), (x8 + 30, y8), 1, 1.5, (128, 0, 250), 2)
            cv2.putText(output, str(int(angle4)), (x11 + 30, y11), 1, 1.5, (128, 0, 250), 2)

            cv2.imshow("output", output)
        cv2.imshow("Frame", frame)
        if cv2.waitKey(1) & 0XFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
# Nombre del archivo CSV
nombre_archivo = "datosConRestricciones.csv"

# Escribir en el archivo CSV
with open(nombre_archivo, mode='w', newline='') as archivo_csv:
    escritor = csv.writer(archivo_csv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for fila in Data:
        escritor.writerow(fila)

print(f"Se han guardado los datos en {nombre_archivo}")