from tkinter import *
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


def iniciar():
    global cap
    cap = p2
    visualizar()

def Finalizar():
    root.quit()



root = Tk()
cap = None

btnInicio = Button(root, text="Iniciar", command=iniciar)
btnInicio.grid(column=0, row=0, padx=5, pady=5)

btnFinal = Button(root, text="Finalizar", command=Finalizar)
btnFinal.grid(column=1, row=0, padx=5, pady=5)

lblVideo = Label(root)
lblVideo.grid(column=0, row=1, columnspan=2)

root.mainloop()