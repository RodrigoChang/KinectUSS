import socket

def recibir_datos():
    # Configurar el servidor
    host = "localhost"
    puerto = 5052

    # Crear un socket UDP
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((host, puerto))

    print(f"Servidor escuchando en {host}:{puerto}")

    try:
        while True:
            # Recibir datos del cliente
            data, address = server_socket.recvfrom(1024)
            decoded_data = data.decode("utf-8")

            # Mostrar los datos recibidos
            print(f"Datos recibidos de {address}: {decoded_data}")

            # Puedes realizar otras acciones con los datos recibidos según tus necesidades

    except KeyboardInterrupt:
        print("\nServidor detenido por el usuario.")
    finally:
        # Cerrar el socket al finalizar
        server_socket.close()

if __name__ == "__main__":
    recibir_datos()
