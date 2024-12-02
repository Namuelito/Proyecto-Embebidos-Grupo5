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
const char* ssid = "iPhone de Manuel";
const char* password = "Maeg1234.";

// URL del servidor (cambia según tu configuración)
const char* serverUrl = "http://172.20.10.2/Registro/verificar.php";

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

    // Datos enviados al servidor
    String httpRequestData = "uid=" + uid;
    Serial.println("Enviando datos al servidor: " + httpRequestData);

    // Enviar la solicitud POST
    int httpResponseCode = http.POST(httpRequestData);

    // // Verificar el código de respuesta del servidor
    // if (httpResponseCode > 0) {
    //   // Imprimir el código de respuesta
    //   Serial.print("Código de respuesta HTTP: ");
    //   Serial.println(httpResponseCode);

      // // Leer la respuesta del servidor
      // String respuesta = http.getString();
      // Serial.print("Respuesta del servidor: ");
      // Serial.println(respuesta);

      // Analizar la respuesta
      // if (respuesta == "ACCESO_CONCEDIDO") {
      //   return true;
      // } else {
      //   return false;
      // }
    } else {
      // Imprimir el error si no hay respuesta
      Serial.print("Error en la conexión al servidor: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi no conectado.");
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
  Serial.println("UID enviado al servidor: " + tarjetaUID);


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
