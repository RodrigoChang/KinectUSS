import mediapipe as mp
import cv2
import numpy as np
import uuid
import os
import math

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

cap = cv2.VideoCapture(0)

def calculate_angle(x1, y1, x2, y2, x3, y3):
    """Calcula el ángulo entre tres puntos (en grados)."""
    angle_rad = math.atan2(y3 - y2, x3 - x2) - math.atan2(y1 - y2, x1 - x2)
    angle_deg = math.degrees(angle_rad)
    return angle_deg

# Crear ventana de datos solo con los ángulos
angle_data = np.zeros((150, 300, 3), dtype=np.uint8)
angle_data.fill(255)  # Fondo blanco
cv2.putText(angle_data, f'Ángulos - Izquierda', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
cv2.putText(angle_data, f'Ángulos - Derecha', (10, 100), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)

# Crear ventanas
cv2.namedWindow('Hand Tracking', cv2.WINDOW_NORMAL)
cv2.namedWindow('Angle Data', cv2.WINDOW_NORMAL)

with mp_hands.Hands(min_detection_confidence=0.8, min_tracking_confidence=0.5, max_num_hands=2) as hands: 
    while cap.isOpened():
        ret, frame = cap.read()
        
        # BGR 2 RGB
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        # Flip on horizontal
        image = cv2.flip(image, 1)
        
        # Set flag
        image.flags.writeable = False
        
        # Detections
        results = hands.process(image)
        
        # Set flag to true
        image.flags.writeable = True
        
        # RGB 2 BGR
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        
        # Detections
        print(results)
        
        # Variables para almacenar ángulos
        angle_thumb_left, angle_thumb_right = 0, 0
        angle_left, angle_right = 0, 0

        # Rendering results
        if results.multi_hand_landmarks:
            for num, hand in enumerate(results.multi_hand_landmarks):
                mp_drawing.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS, 
                                        mp_drawing.DrawingSpec(color=(121, 22, 76), thickness=2, circle_radius=4),
                                        mp_drawing.DrawingSpec(color=(250, 44, 250), thickness=2, circle_radius=2),
                                         )
                
                # Calcular ángulos para cada dedo
                if len(hand.landmark) == 21:
                    # Determinar si la mano es izquierda o derecha
                    if hand.landmark[0].x < hand.landmark[9].x:
                        side = 'Izquierda'
                        angle_text_position = (10, 30)
                    else:
                        side = 'Derecha'
                        angle_text_position = (10, image.shape[0] - 10)

                    # Dedo pulgar
                    if side == 'Izquierda':
                        angle_thumb_left = calculate_angle(hand.landmark[2].x, hand.landmark[2].y,
                                                           hand.landmark[1].x, hand.landmark[1].y,
                                                           hand.landmark[0].x, hand.landmark[0].y)
                    else:
                        angle_thumb_right = calculate_angle(hand.landmark[2].x, hand.landmark[2].y,
                                                            hand.landmark[1].x, hand.landmark[1].y,
                                                            hand.landmark[0].x, hand.landmark[0].y)

                    # Otros dedos
                    for i in range(5, 21):
                        if i % 4 == 0:
                            angle = calculate_angle(hand.landmark[i].x, hand.landmark[i].y,
                                                    hand.landmark[i-1].x, hand.landmark[i-1].y,
                                                    hand.landmark[i-2].x, hand.landmark[i-2].y)
                            if side == 'Izquierda':
                                angle_left = angle
                            else:
                                angle_right = angle
                            cv2.putText(image, f'Ángulo {side} Dedo {i//4}: {int(angle)}', (angle_text_position[0], angle_text_position[1] + (i//4) * 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
            
        # Mostrar resultados en la ventana de tracking
        cv2.imshow('Hand Tracking', image)

        # Mostrar ángulos en la ventana de datos
        cv2.putText(angle_data, f'Ángulo Izq Pulgar: {int(angle_thumb_left)}', (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Izq Dedo 1: {int(angle_left)}', (10, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Izq Dedo 2: {int(angle_left)}', (10, 100), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Izq Dedo 3: {int(angle_left)}', (10, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Izq Dedo 4: {int(angle_left)}', (10, 140), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)

        cv2.putText(angle_data, f'Ángulo Der Pulgar: {int(angle_thumb_right)}', (10, 160), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Der Dedo 1: {int(angle_right)}', (10, 180), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Der Dedo 2: {int(angle_right)}', (10, 200), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Der Dedo 3: {int(angle_right)}', (10, 220), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
        cv2.putText(angle_data, f'Ángulo Der Dedo 4: {int(angle_right)}', (10, 240), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)

        cv2.imshow('Angle Data', angle_data)

        # Save our image    
        cv2.imwrite(os.path.join('Output Images', '{}.jpg'.format(uuid.uuid1())), image)

        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()
