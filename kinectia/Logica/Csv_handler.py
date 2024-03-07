import pandas as pd
import csv

columnas = ['left shoulder', 'left elbow', 'left wrist','left hip','left knee', 'left ankle', 'left heel', 'left foot index'
                  'right shoulder', 'right elbow', 'right wrist','right hip','right knee', 'right ankle', 'right heel', 'right foot index']

def save_frame(linea, lista):
    xs = []
    ys = []
    zs = []
    listafinal = []
    for i in linea.split(','):
        zs.append(float(i))
    for string in lista:
        numbers = string.split(',')
        num1 = int(numbers[0])
        num2 = int(numbers[1])
        xs.append(num1)
        ys.append(num2)

    with open('Test1.csv', 'a', newline='') as csvfile:
    # Create a CSV writer object with ";" as the delimiter
        csv_writer = csv.writer(csvfile)
        print(xs)
        print(ys)
        print(zs)
        for x in range(16):
            listafinal.append(xs[x])
            listafinal.append(ys[x])
            listafinal.append(int(zs[x]))
        csv_writer.writerow(listafinal)

def read_frame():
    data = pd.read_csv('Test1.csv', low_memory=False)
    return data