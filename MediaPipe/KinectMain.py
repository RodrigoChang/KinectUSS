#Version 1.0
#importes necesarios
import cv2
import mediapipe as mp
from threading import Thread
import time
#Comentar para que parta sin socket
import susweb as sus
import numpy as np
from math import acos, degrees
import Bodytracking as BT
#Linux
Video= cv2.VideoCapture(0, cv2.CAP_V4L)
Streaming= cv2.VideoCapture('udp://0.0.0.0:6000?overrun_nonfatal=1', cv2.CAP_V4L)
Mode=Video
BT.Bodytracking(Mode)




#llamar a los modelos 
