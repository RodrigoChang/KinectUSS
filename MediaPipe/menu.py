import cv2
import imutils
import tkinter as tk
from PIL import Image, ImageTk
from tkinter import font
from config import COLOR_BARRA_SUPERIOR, COLOR_CUERPO_PRINCIPAL,COLOR_MENU,COLOR_MENU_CURSOR_ENCIMA


class MaestroDesign(tk.Tk):

    def __init__(self):
        super().__init__()
        self.config_window()
        self.paneles()
        self.controles_barra_superior()
        self.controles_cuerpo()
        self.botones_arranque()

    video = None
    etiqueta = None  # Inicializamos la variable de etiqueta

    def config_window(self):
        self.title("KINECTUSS")
        w, h = 1024, 600
        self.centrar_ventana(w, h)

    def paneles(self):

        # Creamos los paneles (barras)
        
        self.menu_superior = tk.Frame(self, bg=COLOR_MENU, height=40)
        self.menu_superior.pack(side=tk.TOP, fill='both', expand=False)

        self.cuerpo_principal = tk.Frame(
            self, bg=COLOR_CUERPO_PRINCIPAL, width=150)
        self.cuerpo_principal.pack(side=tk.BOTTOM, fill='both', expand=True)

    def centrar_ventana(self, width, height):
        screen_width = self.winfo_screenwidth()
        screen_height = self.winfo_screenheight()

        x = (screen_width / 2) - (width / 2)
        y = (screen_height / 2) - (height / 2)

        self.geometry(f'{width}x{height}+{int(x)}+{int(y)}')

    def controles_cuerpo(self):
        label = tk.Label(self.cuerpo_principal, text="", bg=COLOR_CUERPO_PRINCIPAL)
        label.place(x=0, y=0, relwidth=1, relheight=1)

    def controles_barra_superior(self):

        # Configuramos la barra de menú
        ancho_menu = 15
        alto_menu = 2
        font_awesome = font.Font(family='FontAwesome', size=12)

        # Creamos los botones
        self.buttonInicio = tk.Button(self.menu_superior)
        self.buttonVideo = tk.Button(self.menu_superior)
        self.buttonStreaming = tk.Button(self.menu_superior)
        self.buttonExit = tk.Button(self.menu_superior)

        buttons_info = [

            (self.buttonInicio, "INICIO",self),
            (self.buttonVideo, "VIDEO",self),
            (self.buttonStreaming, "STREAMING",self),
            (self.buttonExit, "EXIT",self.destroy)
        ]

        for button, text, comando in buttons_info:
            self.configurar_boton_menu(button, text, font_awesome, ancho_menu, alto_menu, comando)

    def configurar_boton_menu(self, button, text, font_awesome, ancho_menu, alto_menu, comando):
        button.config(text=f"{text}", anchor="c", font=font_awesome,
                     bd=0, bg=COLOR_MENU, fg="white", width=ancho_menu, height=alto_menu, command= comando)
        button.pack(side=tk.LEFT, padx=90, anchor="c")

        self.bind_hover_events(button)

    def bind_hover_events(self, button):
        # Asociar eventos enter y leave con la función dinámica
        button.bind("<Enter>", lambda event: self.on_enter(event, button))
        button.bind("<Leave>", lambda event: self.on_leave(event, button))

    def on_enter(self, event, button):
        # Cambiar estilo al pasar el ratón por encima
        button.config(bg=COLOR_MENU_CURSOR_ENCIMA, fg="white")

    def on_leave(self, event, button):
        # Cambiar estilo al salir el ratón
        button.config(bg=COLOR_MENU, fg="white")

    def iniciar_video(self):
        self.video = cv2.VideoCapture(0)
        self.iniciar()

    def iniciar(self):
        global video
        if self.video:
            ret, frame = self.video.read()
            if ret:

                frame = imutils.resize(frame, width=640)
                frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

                # Aplicar filtro de mediana para reducir el ruido

                frame = cv2.medianBlur(frame, ksize=5)
                img = Image.fromarray(frame)
                image = ImageTk.PhotoImage(image=img)
                self.etiqueta.configure(image=image)
                self.etiqueta.image = image
                self.after(10, self.iniciar)

    def quitar(self):
        global video
        self.etiqueta.place_forget()
        self.video.release()

    def botones_arranque(self):
        
        # Accedemos al atributo de la clase
        self.etiqueta = tk.Label(self, bg="white")
        self.etiqueta.place(x=187, y=75)

        arranque = tk.Button(self, text="Iniciar video", bg=COLOR_CUERPO_PRINCIPAL, relief="flat",
                             cursor="hand2", command=self.iniciar_video, width=15, height=2, font=("Calisto MT", 12, "bold"))
        arranque.place(x=200, y=590)

        detener = tk.Button(self, text="Detener video", bg=COLOR_CUERPO_PRINCIPAL, relief="flat",
                             cursor="hand2", command=self.quitar, width=15, height=2, font=("Calisto MT", 12, "bold"))
        detener.place(x=640, y=590)        

# Creamos una instancia de la clase y la ejecutamos

app = MaestroDesign()
app.mainloop()