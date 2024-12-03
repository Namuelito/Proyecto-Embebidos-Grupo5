#include <SPI.h>
#include <MFRC522.h>

// Pines del lector RFID de entrada
#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Comunicación serial
  SPI.begin();        // Inicializar SPI
  rfid.PCD_Init();    // Inicializar el lector RFID

  Serial.println("Lector RFID listo. Acerca una tarjeta...");
}

void loop() {
  // Verificar si hay una nueva tarjeta presente
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Leer el UID de la tarjeta
  Serial.print("UID de la tarjeta: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX); // Mostrar en hexadecimal
    Serial.print(" ");
  }
  Serial.println();

  // Detener lectura de la tarjeta
  rfid.PICC_HaltA();
}
