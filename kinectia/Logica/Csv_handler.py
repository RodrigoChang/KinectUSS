import pandas as pd
import csv

zs = []
columnas = ['left shoulder', 'left elbow', 'left wrist','left hip','left knee', 'left ankle', 'left heel', 'left foot index'
                  'right shoulder', 'right elbow', 'right wrist','right hip','right knee', 'right ankle', 'right heel', 'right foot index']

def save_frame(str, lista):
    for i in str.split(','):
        zs.append(float(i))
    with open ('movimiento.csv','a', newline='') as tabla:
        csv_writter = csv.writer(tabla)
        #csv_writter.writerow(columnas)
        csv_writter.writerow(lista)
        #csv_writter.close()
        
def read_frame():
    data = pd.read_csv('movimiento.csv', low_memory=False)
    return data