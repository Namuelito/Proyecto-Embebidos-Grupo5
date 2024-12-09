import csv
import tkinter as tk
from tkinter import messagebox
import serial
import threading
from datetime import datetime

# Configuración
archivo_usuarios = "./usuarios.csv"
archivo_eventos = "./eventos.csv"
archivo_ultrasonico = "./ultrasonico.csv"
puerto_arduino_uno = "COM5"  # Puerto del Arduino Uno
puerto_arduino_mega = "COM6"  # Puerto del Arduino Mega
baudrate = 9600

# Inicializar CSV si no existen
def inicializar_csv(nombre_archivo, encabezados):
    try:
        with open(nombre_archivo, mode='x', newline='', encoding='utf-8') as archivo:
            escritor = csv.writer(archivo)
            escritor.writerow(encabezados)
    except FileExistsError:
        pass

inicializar_csv(archivo_usuarios, ["UID", "Nombre", "Permitir Entrada", "Permitir Salida"])
inicializar_csv(archivo_eventos, ["UID", "Fecha", "Tipo", "Evento"])
inicializar_csv(archivo_ultrasonico, ["Fecha", "Evento"])

# Conectar con Arduinos
arduinos = {}
try:
    arduinos["uno"] = serial.Serial(puerto_arduino_uno, baudrate, timeout=1)
    print(f"Conectado a Arduino Uno en {puerto_arduino_uno}")
except Exception as e:
    print(f"Error al conectar con Arduino Uno: {e}")

try:
    arduinos["mega"] = serial.Serial(puerto_arduino_mega, baudrate, timeout=1)
    print(f"Conectado a Arduino Mega en {puerto_arduino_mega}")
except Exception as e:
    print(f"Error al conectar con Arduino Mega: {e}")

# Funciones del sistema
def leer_csv(nombre_archivo):
    with open(nombre_archivo, mode='r', newline='', encoding='utf-8') as archivo:
        return list(csv.reader(archivo))

def escribir_csv(nombre_archivo, datos):
    with open(nombre_archivo, mode='w', newline='', encoding='utf-8') as archivo:
        escritor = csv.writer(archivo)
        escritor.writerows(datos)

def agregar_fila_csv(nombre_archivo, fila):
    with open(nombre_archivo, mode='a', newline='', encoding='utf-8') as archivo:
        escritor = csv.writer(archivo)
        escritor.writerow(fila)

def buscar_usuario(uid):
    usuarios = leer_csv(archivo_usuarios)
    for fila in usuarios:
        if fila[0] == uid:
            return fila
    return None

def registrar_evento(uid, tipo):
    fecha_actual = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    agregar_fila_csv(archivo_eventos, [uid, fecha_actual, tipo, "Acceso autorizado"])
    actualizar_eventos(f"Evento registrado: UID={uid}, Tipo={tipo}, Fecha={fecha_actual}")

def registrar_proximidad():
    fecha_actual = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    agregar_fila_csv(archivo_ultrasonico, [fecha_actual, "Proximidad detectada"])
    actualizar_eventos(f"Evento de proximidad registrado: {fecha_actual}")

def validar_acceso(uid, tipo):
    usuario = buscar_usuario(uid)
    if usuario:
        permitir_entrada = usuario[2].lower() == "si"
        permitir_salida = usuario[3].lower() == "si"
        if (tipo == "Entrada" and permitir_entrada) or (tipo == "Salida" and permitir_salida):
            registrar_evento(uid, tipo)
            arduinos["mega"].write((tipo + "\n").encode())
            actualizar_eventos(f"Acceso autorizado para UID {uid} ({tipo}).")
        else:
            arduinos["mega"].write("Acceso denegado\n".encode())
            actualizar_eventos(f"UID {uid} no autorizado para {tipo}.")
    else:
        arduinos["mega"].write("Acceso denegado\n".encode())
        actualizar_eventos(f"UID {uid} no registrado.")
    
    # Asegurarse de que no se queden datos pendientes
    arduinos["mega"].flush()  # Limpiar buffer de salida

def registrar_usuario(uid):
    def guardar():
        nombre = entry_nombre.get()
        if not nombre:
            messagebox.showerror("Error", "El nombre no puede estar vacío.")
            return

        agregar_fila_csv(archivo_usuarios, [uid, nombre, "Si", "Si"])  # Permitir Entrada y Salida
        ventana.destroy()
        actualizar_eventos(f"Nuevo usuario registrado: UID={uid}, Nombre={nombre}")

    ventana = tk.Toplevel(root)
    ventana.title("Registrar Usuario")
    ventana.geometry("300x200")

    tk.Label(ventana, text=f"UID: {uid}").pack(pady=5)
    tk.Label(ventana, text="Nombre:").pack(pady=5)
    entry_nombre = tk.Entry(ventana)
    entry_nombre.pack(pady=5)
    tk.Button(ventana, text="Guardar", command=guardar).pack(pady=10)

def actualizar_eventos(texto):
    text_eventos.insert(tk.END, texto + "\n")
    text_eventos.see(tk.END)

# Procesar datos del Arduino Uno
def procesar_uno():
    while True:
        if arduinos["uno"].in_waiting > 0:
            uid = arduinos["uno"].readline().decode().strip()
            if not buscar_usuario(uid):
                registrar_usuario(uid)

# Procesar datos del Arduino Mega
def procesar_mega():
    while True:
        if arduinos["mega"].in_waiting > 0:
            data = arduinos["mega"].readline().decode().strip()
            print(f"Datos recibidos desde Arduino Mega: {data}")

            if data.startswith("ULTRASONICO"):
                registrar_proximidad()
            else:
                try:
                    uid, tipo = data.split()
                    validar_acceso(uid, tipo)
                except ValueError:
                    actualizar_eventos(f"{data}")

# GUI principal
root = tk.Tk()
root.title("Sistema de Control de Acceso")
root.geometry("500x400")

tk.Label(root, text="Sistema de Control de Acceso", font=("Arial", 16)).pack(pady=10)

frame_eventos = tk.LabelFrame(root, text="Eventos", padx=10, pady=10)
frame_eventos.pack(fill="both", expand="yes", padx=10, pady=10)
text_eventos = tk.Text(frame_eventos, height=15)
text_eventos.pack(fill="both", expand="yes")

# Hilos para procesar datos
if "uno" in arduinos:
    hilo_uno = threading.Thread(target=procesar_uno, daemon=True)
    hilo_uno.start()

if "mega" in arduinos:
    hilo_mega = threading.Thread(target=procesar_mega, daemon=True)
    hilo_mega.start()

root.mainloop()
