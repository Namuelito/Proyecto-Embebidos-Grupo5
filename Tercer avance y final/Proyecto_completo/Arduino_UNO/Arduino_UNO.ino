#include <SPI.h>
#include <MFRC522.h>

// Pines para el lector RFID
#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Comunicación con Python
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Arduino Uno listo para registrar tarjetas.");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = leerUID(rfid);
    Serial.println(uid); // Enviar UID al programa Python
    delay(2000);         // Evitar lecturas consecutivas de la misma tarjeta
    rfid.PICC_HaltA();   // Detener lectura
  }
}

String leerUID(MFRC522& lector) {
  String uid = "";
  for (byte i = 0; i < lector.uid.size; i++) {
    uid += String(lector.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}
