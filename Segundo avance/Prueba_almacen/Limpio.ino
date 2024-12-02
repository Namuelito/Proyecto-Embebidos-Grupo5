#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <ESP32Servo.h> // Usar ESP32Servo en lugar de Servo.h

// Pines para el RFID
#define RST_PIN 22
#define SS_PIN 21

// Configuración del servo
Servo myServo; // Usar la clase Servo de ESP32Servo
const int servoPin = 15;

// Datos de red WiFi
const char* ssid = "HECTOR_2.4";
const char* password = "HECTOR2295874";

// URL del servidor (cambia según tu configuración)
const char* serverUrl = "http://192.168.1.11/Registro/verificar.php";

// Inicializar RFID
MFRC522 rfid(SS_PIN, RST_PIN);

// UID de la tarjeta
String tarjetaUID;

// Función para conectarse a WiFi
void conectarWiFi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println("IP asignada: ");
  Serial.println(WiFi.localIP());
}

// Función para enviar UID al servidor y verificar acceso
bool verificarAcceso(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "uid=" + uid;
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String respuesta = http.getString();
      http.end();
      return (respuesta == "ACCESO_CONCEDIDO");
    } else {
      Serial.print("Error en la conexión: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Inicializar comunicación SPI
  rfid.PCD_Init(); // Inicializar RFID
  myServo.attach(servoPin); // Conectar el servomotor
  myServo.write(0); // Asegurarse de que la puerta esté cerrada

  conectarWiFi();
}

void loop() {
  // Verificar si hay una tarjeta presente
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Leer el UID de la tarjeta
  tarjetaUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    tarjetaUID += String(rfid.uid.uidByte[i], HEX);
  }
  tarjetaUID.toUpperCase();

  Serial.println("Tarjeta detectada: " + tarjetaUID);

  // Verificar acceso
  if (verificarAcceso(tarjetaUID)) {
    Serial.println("Acceso concedido");
    myServo.write(90); // Abrir la puerta
    delay(3000); // Mantener la puerta abierta
    myServo.write(0); // Cerrar la puerta
  } else {
    Serial.println("Acceso denegado");
  }

  // Detener la lectura de tarjetas
  rfid.PICC_HaltA();
}
