#include <SPI.h>
#include <MFRC522.h>

// Pines del lector RFID de entrada
#define RST_PIN_ENTRADA 9
#define SS_PIN_ENTRADA 10

// Pines del lector RFID de salida
#define RST_PIN_SALIDA 8
#define SS_PIN_SALIDA 7

MFRC522 rfidEntrada(SS_PIN_ENTRADA, RST_PIN_ENTRADA);
MFRC522 rfidSalida(SS_PIN_SALIDA, RST_PIN_SALIDA);

void setup() {
  Serial.begin(9600);
  SPI.begin();

  // Configurar pines SS
  pinMode(SS_PIN_ENTRADA, OUTPUT);
  pinMode(SS_PIN_SALIDA, OUTPUT);

  // Desactivar ambos lectores al inicio
  digitalWrite(SS_PIN_ENTRADA, HIGH);
  digitalWrite(SS_PIN_SALIDA, HIGH);

  // Inicializar los lectores RFID
  rfidEntrada.PCD_Init();
  rfidSalida.PCD_Init();

  Serial.println("Sistema RFID listo.");
}

void loop() {
  // Activar lector de entrada
  digitalWrite(SS_PIN_SALIDA, HIGH);  // Desactivar lector de salida
  digitalWrite(SS_PIN_ENTRADA, LOW); // Activar lector de entrada
  if (rfidEntrada.PICC_IsNewCardPresent() && rfidEntrada.PICC_ReadCardSerial()) {
    Serial.print("Entrada UID: ");
    printUID(rfidEntrada);
    rfidEntrada.PICC_HaltA();
  }
  digitalWrite(SS_PIN_ENTRADA, HIGH); // Desactivar lector de entrada

  // Activar lector de salida
  digitalWrite(SS_PIN_ENTRADA, HIGH); // Desactivar lector de entrada
  digitalWrite(SS_PIN_SALIDA, LOW);  // Activar lector de salida
  if (rfidSalida.PICC_IsNewCardPresent() && rfidSalida.PICC_ReadCardSerial()) {
    Serial.print("Salida UID: ");
    printUID(rfidSalida);
    rfidSalida.PICC_HaltA();
  }
  digitalWrite(SS_PIN_SALIDA, HIGH); // Desactivar lector de salida
}

void printUID(MFRC522& lector) {
  for (byte i = 0; i < lector.uid.size; i++) {
    Serial.print(lector.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
