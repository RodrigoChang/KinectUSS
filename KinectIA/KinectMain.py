#Version 1.2
#importes necesarios
from LogicAI import Body_tracking as BT
#import LogicAI.Bodyanglecalculator as BTA
#import LogicAI.Handstracking as HT
import configparser
import argparse

def start():
    parser = argparse.ArgumentParser(description='sUs.')
    config = configparser.ConfigParser()

    parser.add_argument('--gui', action='store_true', help='Disables the GUI')
    parser.add_argument('--ip', type=str, help='Set IP')
    parser.add_argument('--view', type=str, help='Set Viewer')
    parser.add_argument('--model', type=str, help='Set Model')

    #CONFIG
    config.read('config.ini')
    gui = config.get('Settings', 'GUI')

    #ARGS
    args = parser.parse_args()
    gui = args.gui

    Input_IP = args.ip if args.ip != None else config.get('Settings', 'IP')
    View = args.view if args.view != None else config.get('Settings', 'View')
    model = args.model if args.model != None else config.get('Settings', 'Model')

    if View == "Body":
        if model == "bt":
            BT.Bodytracking(Input_IP)
        if model == "bta":    
            BTA.Bodyanglecalculator(Input_IP)
    if View == "Hands":
        HT.Hands(Input_IP)

if __name__ == "__main__":
    start()