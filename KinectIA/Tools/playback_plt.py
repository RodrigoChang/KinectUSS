import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import argparse

def playback():

    parser = argparse.ArgumentParser(description='sUs.')
    parser.add_argument('file_path', type=str, help='Path of the file')
    args = parser.parse_args()

    archivo_csv = args.file_path

    if archivo_csv != None:
        try:

            # Definir las claves de las columnas
            columnas = ['left shoulder', 'left elbow', 'left wrist', 'left hip', 'left knee', 'left ankle', 'left heel', 'left foot index',
                        'right shoulder', 'right elbow', 'right wrist', 'right hip', 'right knee', 'right ankle', 'right heel', 'right foot index']

            # Crear la figura 3D
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')

            # Conectar los puntos correspondientes a las partes del cuerpo
            connections = [
                ('left shoulder', 'left elbow'), ('left elbow', 'left wrist'), ('left shoulder', 'left hip'),
                ('left hip', 'left knee'), ('left knee', 'left ankle'), ('left ankle', 'left heel'), ('left heel', 'left foot index'),
                ('right shoulder', 'right elbow'), ('right elbow', 'right wrist'), ('right shoulder', 'right hip'),
                ('right hip', 'right knee'), ('right knee', 'right ankle'), ('right ankle', 'right heel'), ('right heel', 'right foot index'),
                ('left shoulder', 'right shoulder'), ('left hip', 'right hip')
            ]

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

                    # Graficar los puntos
                    for joint, coordinates in joint_data.items():
                        x, y, z = coordinates
                        ax.scatter(x, y, z, label=joint)

                    # Conectar los puntos correspondientes a las partes del cuerpo
                    for connection in connections:
                        joint1, joint2 = connection
                        ax.plot([joint_data[joint1][0], joint_data[joint2][0]],
                                [joint_data[joint1][1], joint_data[joint2][1]],
                                [joint_data[joint1][2], joint_data[joint2][2]], linestyle='-', color='black')

                    # Etiquetas de los ejes
                    ax.set_xlabel('Eje X')
                    ax.set_ylabel('Eje Y')
                    ax.set_zlabel('Eje Z')

                    # Mostrar el gráfico
                    ax.legend()
                    plt.draw()
                    plt.pause(0.1)  # Pausa para permitir la actualización del gráfico

                    # Limpiar el gráfico para la próxima iteración
                    ax.cla()

            # Mantener el gráfico abierto al final
            plt.show()
        except NameError:
            "Archivo no valido"
    else:
        "Dale una direccion"

if __name__ == "__main__":
    playback()
