#Version 1.2
#importes necesarios
from multiprocessing import Process, cpu_count
from threading import Thread
import time
import os
import sys
#sys.path.append('../')
#PROJECT_ROOT = os.path.abspath(os.path.join(
#                  os.path.dirname(__file__), 
#                  os.pardir)
#)
sys.path.append(PROJECT_ROOT)
import Logica.Body_tracking as BT
import Logica.Bodyanglecalculator as BTA
import Logica.Handstracking as HT
#import utiles.PC_camer_streaming as Video_Stream
#import utiles.Frame_receptor as Server_Connect
#Linux

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

