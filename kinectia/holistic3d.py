import cv2
import mediapipe as mp
import numpy as np
from mayavi import mlab

# Configurar la detección de landmarks en 3D
mp_holistic = mp.solutions.holistic
holistic = mp_holistic.Holistic()

# Configurar la captura de video
cap = cv2.VideoCapture(0)  # Puedes cambiar a un archivo de video o cámara específica según tu caso

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    # Convertir la imagen a color
    color_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Realizar la detección de landmarks
    results = holistic.process(image=color_frame)

    # Obtener los landmarks en 3D
    landmarks_3d = []
    if results.pose_landmarks:
        for landmark in results.pose_landmarks.landmark:
            x, y, z = landmark.x, landmark.y, landmark.z
            landmarks_3d.append([x, y, z])

    # Crear un avatar 3D
    mlab.clf()
    landmarks_3d_array = np.array(landmarks_3d)
    mlab.points3d(landmarks_3d_array[:, 0], landmarks_3d_array[:, 1], landmarks_3d_array[:, 2],
                  color=(1, 0, 0), mode='sphere', scale_factor=0.1)

    # Mostrar la visualización 3D
    mlab.show()

    # Presiona 'Esc' para salir
    if cv2.waitKey(1) & 0xFF == 27:
        break

# Liberar recursos
cap.release()
cv2.destroyAllWindows()
