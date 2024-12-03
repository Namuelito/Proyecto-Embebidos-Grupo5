#include <SPI.h>
#include <MFRC522.h>
#include <VarSpeedServo.h>

// Pines para el lector RFID de entrada
#define RST_PIN_ENTRADA 9
#define SS_PIN_ENTRADA 10

// Pines para el lector RFID de salida
#define RST_PIN_SALIDA 8
#define SS_PIN_SALIDA 7

// Pines Sensor Ultrasónico
#define TRIG_PIN 6
#define ECHO_PIN 5

// Servomotor
#define SERVO_PIN 4

MFRC522 rfidEntrada(SS_PIN_ENTRADA, RST_PIN_ENTRADA);
MFRC522 rfidSalida(SS_PIN_SALIDA, RST_PIN_SALIDA);
VarSpeedServo servo;

void setup() {
  Serial.begin(9600); // Comunicación con Python
  SPI.begin();        // Inicializar SPI

  // Inicializar lectores RFID
  rfidEntrada.PCD_Init();
  rfidSalida.PCD_Init();

  // Inicializar servomotor
  servo.attach(SERVO_PIN);
  servo.write(0, 30, true); // Asegúrate de que la puerta está cerrada (posición 0°) con velocidad 30

  // Configuración del sensor ultrasónico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Sistema de control de acceso listo.");
}

void loop() {
  // Sensor ultrasónico: Detectar acercamiento
  long distancia = medirDistancia();
  if (distancia > 0 && distancia < 20) { // Ajustar el umbral según sea necesario
    Serial.println("ULTRASONICO");
    delay(2000); // Evitar múltiples registros consecutivos
  }

  // Lector RFID de entrada
  if (rfidEntrada.PICC_IsNewCardPresent() && rfidEntrada.PICC_ReadCardSerial()) {
    manejarRFID(rfidEntrada, "Entrada");
  }

  // Lector RFID de salida
  if (rfidSalida.PICC_IsNewCardPresent() && rfidSalida.PICC_ReadCardSerial()) {
    manejarRFID(rfidSalida, "Salida");
  }
}

// Función para medir distancia con sensor ultrasónico
long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracion * 0.034 / 2;
  return distancia;
}

// Función para manejar RFID
void manejarRFID(MFRC522& lector, String tipoAcceso) {
    String uid = leerUID(lector);
    Serial.println(uid + " " + tipoAcceso); // Enviar UID y tipo al programa Python
    delay(100); // Pequeño retardo para asegurar el envío completo

    while (!Serial.available()); // Esperar respuesta
    String respuesta = Serial.readString(); // Leer respuesta desde Python
    respuesta.trim();                       // Eliminar espacios en blanco

    if (respuesta == tipoAcceso) {
        abrirPuerta();
    }

    lector.PICC_HaltA(); // Detener lectura
}


// Función para leer UID de la tarjeta RFID
String leerUID(MFRC522& lector) {
  String uid = "";
  for (byte i = 0; i < lector.uid.size; i++) {
    uid += String(lector.uid.uidByte[i], HEX);
  }
  uid.toUpperCase(); // Convertir a mayúsculas
  return uid;
}

// Función para abrir la puerta con velocidad controlada
void abrirPuerta() {
  servo.write(90, 20, true); // Mover a 90° con velocidad 20
  delay(3000);               // Mantener abierta 3 segundos
  servo.write(0, 20, true);  // Volver a 0° con velocidad 20
}
