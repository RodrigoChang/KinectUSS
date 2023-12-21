import csv
import cv2
import numpy as np

# Nombre del archivo CSV
archivo_csv = 'Test1.csv'  # Reemplaza 'tu_archivo.csv' con el nombre de tu archivo CSV

# Definir las claves de las columnas
columnas = ['left shoulder', 'left elbow', 'left wrist', 'left hip', 'left knee', 'left ankle', 'left heel', 'left foot index',
            'right shoulder', 'right elbow', 'right wrist', 'right hip', 'right knee', 'right ankle', 'right heel', 'right foot index']

# Configuración del tamaño de la imagen
img_size = (1200, 900)  # Aumentar el tamaño de la imagen

# Crear una imagen en blanco
img = np.ones((img_size[1], img_size[0], 3), dtype=np.uint8) * 255

# Abrir el archivo CSV
with open(archivo_csv, 'r') as csv_file:
    # Crear un lector CSV
    csv_reader = csv.reader(csv_file)

    # Iterar sobre las líneas del archivo CSV
    for row in csv_reader:
        # Convertir las cadenas a números flotantes
        data = list(map(float, row))

        # Separar los datos en un diccionario
        num_joints = len(columnas)
        joint_data = {column: [] for column in columnas}
        for i in range(num_joints):
            start_idx = i * 3
            end_idx = start_idx + 3
            joint_data[columnas[i]] = data[start_idx:end_idx]

        # Escalar las coordenadas a la dimensión de la imagen
        scale_factor = 0.3  # Ajustar el factor de escala
        joint_data_scaled = {key: (int(val[0] * scale_factor), int(val[1] * scale_factor)) for key, val in joint_data.items()}

        # Dibujar puntos en la imagen
        for joint, coordinates in joint_data_scaled.items():
            cv2.circle(img, coordinates, 5, (0, 0, 255), -1)  # Aumentar el tamaño de los puntos
            #cv2.putText(img, joint, coordinates, cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)  # Aumentar el tamaño del texto

        # Dibujar líneas entre puntos correspondientes a las partes del cuerpo
        connections = [('left shoulder', 'left elbow'), ('left elbow', 'left wrist'), ('left shoulder', 'left hip'),
                       ('left hip', 'left knee'), ('left knee', 'left ankle'), ('left ankle', 'left heel'), ('left heel', 'left foot index'),
                       ('right shoulder', 'right elbow'), ('right elbow', 'right wrist'), ('right shoulder', 'right hip'),
                       ('right hip', 'right knee'), ('right knee', 'right ankle'), ('right ankle', 'right heel'), ('right heel', 'right foot index'),
                       ('left shoulder', 'right shoulder'), ('left hip', 'right hip')]

        for connection in connections:
            joint1, joint2 = connection
            cv2.line(img, joint_data_scaled[joint1], joint_data_scaled[joint2], (0, 255, 0), 2)  # Aumentar el grosor de las líneas

        # Mostrar la imagen en cada iteración
        cv2.imshow('Pose Estimation', img)
        cv2.waitKey(200)  # Pausa de 200 milisegundos (ajustar según sea necesario)

        # Limpiar la imagen para la próxima iteración
        img[:] = 255

cv2.destroyAllWindows()
