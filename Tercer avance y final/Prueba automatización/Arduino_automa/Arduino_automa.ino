#include <SPI.h>
#include <MFRC522.h>

// Pines para el lector RFID
#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Comunicación serial
  SPI.begin();        // Inicializar SPI
  rfid.PCD_Init();    // Inicializar lector RFID

  Serial.println("Lector RFID listo. Acerca una tarjeta...");
}

void loop() {
  // Verificar si hay una nueva tarjeta presente
  if (!rfid.PICC_IsNewCardPresent()) {
    return; // Si no hay tarjeta, salir del loop
  }

  // Verificar si se puede leer la tarjeta
  if (!rfid.PICC_ReadCardSerial()) {
    return; // Si no se puede leer, salir del loop
  }

  // Leer y enviar el UID de la tarjeta
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX); // Convertir UID a hexadecimal
  }
  uid.toUpperCase(); // Convertir a mayúsculas

  Serial.println(uid); // Enviar UID al programa Python
  delay(1000);         // Evitar lecturas consecutivas de la misma tarjeta

  rfid.PICC_HaltA(); // Detener lectura de la tarjeta
}
