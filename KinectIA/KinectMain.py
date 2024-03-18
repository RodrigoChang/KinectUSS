#Version 1.2
#importes necesarios
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

