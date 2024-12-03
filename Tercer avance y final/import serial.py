import serial
from openpyxl import load_workbook
from datetime import datetime

# Configuración
archivo = "Control_Acceso.xlsx"  # Nombre del archivo Excel
puerto_arduino = 'COM6'          # Cambiar al puerto correcto
baudrate = 9600                  # Velocidad de comunicación serial

# Inicializar archivo Excel
try:
    wb = load_workbook(archivo)
    hoja_eventos = wb["Eventos"]  # Cargar la hoja 'Eventos'
except Exception as e:
    print(f"Error al cargar el archivo Excel: {e}")
    exit()

# Inicializar comunicación serial con Arduino
try:
    arduino = serial.Serial(puerto_arduino, baudrate, timeout=1)
    print(f"Conectado al Arduino en {puerto_arduino}")
except Exception as e:
    print(f"Error al conectar con Arduino: {e}")
    exit()

# Bucle principal
try:
    while True:
        if arduino.in_waiting > 0:  # Si hay datos disponibles desde Arduino
            data = arduino.readline().decode().strip()  # Leer datos del puerto serial
            print(f"Datos recibidos: {data}")

            # Simular respuesta de Python para prueba
            if data != "ULTRASONICO":  # Evitar procesar eventos ultrasónicos
                try:
                    uid, acceso = data.split()  # UID y tipo de acceso (Entrada/Salida)
                    fecha_actual = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    hoja_eventos.append([uid, fecha_actual, acceso])
                    print(f"Registrando evento: UID={uid}, Fecha={fecha_actual}, Tipo={acceso}")

                    # Guardar cambios en el archivo Excel
                    wb.save(archivo)
                    print("Evento guardado exitosamente.")
                except ValueError:
                    print(f"Formato de datos incorrecto: {data}")
                except Exception as e:
                    print(f"Error al guardar el evento: {e}")
except KeyboardInterrupt:
    print("Programa detenido manualmente.")
except Exception as e:
    print(f"Error inesperado: {e}")
finally:
    arduino.close()
    print("Conexión serial cerrada.")
