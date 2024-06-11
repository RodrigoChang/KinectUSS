import csv
import time
import threading
import random

# Variable global para controlar la ejecución del texto
running = True

def write_coordinates():
    with open("coordenadas.csv", "w", newline='') as csvfile:
        fieldnames = ['x', 'y', 'z']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        while running:
            # Generar coordenadas aleatorias para x, y, z
            x = random.uniform(0, 100)
            y = random.uniform(0, 100)
            z = random.uniform(0, 100)
            
            # Escribir coordenadas en el archivo CSV
            writer.writerow({'x': x, 'y': y, 'z': z})
            
            time.sleep(0.5)

def play():
    global running
    print("Reproduciendo...")
    running = True
    # Iniciar un hilo separado para escribir coordenadas continuamente
    threading.Thread(target=write_coordinates, daemon=True).start()
    # Simulación de alguna acción continua, como reproducción de música
    while True:
        print("En reproducción... Presiona 'p' para pausar o 's' para detener.")
        time.sleep(1)
        action = input()
        if action.lower() == 'p':
            running = False
            pause()
        elif action.lower() == 's':
            running = False
            time.sleep(2)
            print("Guardando exitoso...")
            exit()
            break

def pause():
    global running
    print("Pausado. Presiona 'r' para reanudar o 's' para detener.")
    while True:
        action = input()
        if action.lower() == 'r':
            play()
            break
        elif action.lower() == 's':
            running = False
            time.sleep(2)
            print("Guardando exitoso...")
            exit()
            break

def menu():
    while True:
        print("\nMenú de Control")
        print("1. Play")
        print("2. Salir")
        choice = input("Selecciona una opción: ")

        if choice == '1':
            play()
        elif choice == '2':
            print("Saliendo...")
            break
        else:
            print("Opción no válida, intenta de nuevo.")

if __name__ == "__main__":
    menu()
