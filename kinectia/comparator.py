import pandas as pd
import math

index = 0
distancia = 0 
mantenido = 0
squat = 0
data = pd.read_csv("movimiento.csv")

def buscar_valor():
    valor = 0
    global distancia, index, mantenido
    end_idx = (index + 1)
    lista = data.iloc[index:end_idx].values.reshape(-1, 3)
    xa = lista[3, 0]
    ya = lista[3, 1]
    za = lista[3, 2]
    xb = lista[4, 0]
    yb = lista[4, 1]
    zb = lista[4, 2]
    
    calculo = ((xa-xb)^2)+((ya-yb)^2)+((za-zb)^2)
    if (calculo < 0):
        calculo = calculo * -1
        valor = math.sqrt(calculo)
    else:
        valor = math.sqrt(calculo)
    sus = abs(distancia - valor)
    if (0 < sus < 2 and sus < 20):
        mantenido += 1
        print(f"squateando por {mantenido} frames")
    else:
        distancia = valor
        print(f"La distancia es: {valor}")

def main():
    global distancia, index
    for x in range(321):
        buscar_valor()
        index+= 1

if __name__ == "__main__":
    main()
    