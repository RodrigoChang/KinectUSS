import re
import statistics as stat

file_path = './perf.txt'

listener = []
mat1 = [] 
mat2 = [] 
reg = []
cloud = []
pre = []
imshow = []
ciclo = []
rgb = []
registered = []
resto = []

with open(file_path, 'r') as file:
    content = file.read()

time_groups = content.split('\n\n')

for group in time_groups:
    times = group.strip().split('\n')

    for timing in times:
        if "listener" in timing:
            match = re.search(r'\d+', timing)
            listener.append(int(match.group()))
        elif "base" in timing:
            match = re.search(r'\d+', timing)
            mat1.append(int(match.group()))
        elif "registration" in timing:
            match = re.search(r'\d+', timing)
            reg.append(int(match.group()))
        elif "cloud" in timing:
            match = re.search(r'\d+', timing)
            cloud.append(int(match.group()))
        elif "matrices 2" in timing:
            match = re.search(r'\d+', timing)
            mat2.append(int(match.group()))
        elif "pre display" in timing:
            match = re.search(r'\d+', timing)
            pre.append(int(match.group()))
        elif "imshow" in timing:
            match = re.search(r'\d+', timing)
            imshow.append(int(match.group()))
        elif "ciclo" in timing:
            match = re.search(r'\d+', timing)
            ciclo.append(int(match.group()))
        elif "rgb" in timing:
            match = re.search(r'\d+', timing)
            rgb.append(int(match.group()))
        elif "registered" in timing:
            match = re.search(r'\d+', timing)
            registered.append(int(match.group()))

print("Promedio")
print(f"Listener: {round(stat.mean(listener), 2)}ms")
print(f"Matriz Base: {round(stat.mean(mat1), 2)}ms")
print(f"Registration: {round(stat.mean(reg), 2)}ms")
print(f"Cloud: {round(stat.mean(cloud), 2)}ms")
print(f"Matriz 2: {round(stat.mean(mat2), 2)}ms")
print(f"Pre Display: {round(stat.mean(pre), 2)}ms")
print(f"imshow: {round(stat.mean(imshow), 2)}ms")
print(f"Fin del Ciclo: {round(stat.mean(ciclo), 2)}ms")
print(f"RGB: {round(stat.mean(rgb), 2)}ms")
print(f"Registered Frame: {round(stat.mean(registered), 2)}ms")
print(" ")

for x in range(len(ciclo)):
    resto.append(round(100 * (ciclo[x] - listener[x] - mat2[x] - reg[x] - cloud[x] - mat2[x] - pre[x] - imshow[x]) / ciclo[x], 2))
    listener[x] = round((listener[x] / ciclo[x] * 100), 2)
    mat1[x] = round((mat2[x]/ ciclo[x] * 100), 2)
    reg[x] = round((reg[x]/ ciclo[x] * 100), 2)
    cloud[x] = round((cloud[x]/ ciclo[x] * 100), 2)
    mat2[x] = round((mat2[x]/ ciclo[x] * 100), 2)
    pre[x] = round((pre[x]/ ciclo[x] * 100), 2)
    imshow[x] = round((imshow[x]/ ciclo[x] * 100), 2)
    #rgb[x] = round((rgb[x]/ ciclo[x] * 100), 2)
    #registered[x] = round((registered[x]/ ciclo[x] * 100), 2)

print("Promedio porcentual")
print(f"Listener: {round(stat.mean(listener), 2)}%")
print(f"Matriz Base: {round(stat.mean(mat1), 2)}%")
print(f"Registration: {round(stat.mean(reg), 2)}%")
print(f"Cloud: {round(stat.mean(cloud), 2)}%")
print(f"Matriz 2: {round(stat.mean(mat2), 2)}%")
print(f"Pre Display: {round(stat.mean(pre), 2)}%")
print(f"imshow: {round(stat.mean(imshow), 2)}%")
#print(f"RGB: {stat.mean(rgb)}%")
#print(f"Registered Frame: {stat.mean(registered)}%")
print(f"Resto: {round(stat.mean(resto), 2)}%")