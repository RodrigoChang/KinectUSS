#Version 1.0
#importes necesarios
import cv2
import mediapipe as mp
from threading import Thread
import time
import numpy as np
from math import acos, degrees
import Bodytracking as BT
import Bodyanglecalculator as BTA
import Handstracking as HT
import zmq

#Linux
Input_mode = input("Mode streaming or video: ")   
#Menu Streaming Enable
if Input_mode == "streaming":
    Input_IP= input("IP: ")
    View=input("View Body or Hands: ")
    if View == "Body":
        model=input("Model: ")
        if model == "bt":
            BT.Bodytracking(Input_IP)
        if model == "bta":    
            BTA.Bodyanglecalculator(Input_IP)
    if View == "Hands":
        HT.Hands(Input_IP)
#Menu Video Disable    
if Input_mode == "video":   
    Video= cv2.VideoCapture(0, cv2.CAP_ANY)
    View=input("View: ")
    if View == "Body":
        model=input("Model: ")
        if model == "bt":
            BT.Bodytracking(Video)
        if model == "bta":    
            BTA.Bodyanglecalculator(Video)
    if View == "Hands":
        HT.Hands(Video)


    

  



"""rom tkinter import *
from tkinter import filedialog
from PIL import Image
from PIL import ImageTk
import cv2
import imutils
import p2
def visualizar():
    global cap
    if cap is not None:
        ret, frame = cap.read()
        if ret == True:
            frame = imutils.resize(frame, width=640)
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            im = Image.fromarray(frame)
            img = ImageTk.PhotoImage(image=im)

            lblVideo.configure(image=img)
            lblVideo.image = img
            lblVideo.after(10, visualizar)
        else:
            lblVideo.image = ""
            cap.release()


def Body():
    global cap
    #cap = BT.Bodytracking(Mode)
    visualizar()
def Hands():
    global cap
    #cap = HT.HandsTracking(Mode)     
    visualizar()
def Finalizar():
    root.quit()



root = Tk()
cap = None

btnInicio = Button(root, text="Body", command=Body)
btnInicio.grid(column=0, row=0, padx=5, pady=5)

btnInicio = Button(root, text="Hands", command=Hands)
btnInicio.grid(column=1, row=0, padx=5, pady=5)

btnFinal = Button(root, text="Finalizar", command=Finalizar)
btnFinal.grid(column=2, row=0, padx=5, pady=5)

lblVideo = Label(root)
lblVideo.grid(column=0, row=1, columnspan=3)

root.mainloop()


#llamar a los modelos 
"""