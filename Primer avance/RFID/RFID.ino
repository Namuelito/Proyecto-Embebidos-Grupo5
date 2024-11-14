#include <SPI.h>
#include <MFRC522.h>
#include <VarSpeedServo.h>

#define RST_PIN 9   // Pin para reset del RC522
#define SS_PIN 10   // Pin para SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear el objeto RC522

VarSpeedServo servoMotor;  // Crear objeto VarSpeedServo

byte ActualUID[4];
byte Usuario1[4] = {0x33, 0x49, 0x14, 0xAC};
byte Usuario2[4] = {0xC1, 0x2F, 0xD6, 0x0E};
byte Usuario3[4] = {0x3D, 0xD6, 0x4D, 0x4C};

int posicionEsperada = 0;  // Variable para almacenar la posición esperada del servo

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Control de acceso:");

  servoMotor.attach(3);  // Conecta el VarSpeedServo al pin 3
  servoMotor.write(0);   // Inicializa el servo en posición 0°
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        ActualUID[i] = mfrc522.uid.uidByte[i];
      }
      Serial.print("     ");
      
      if (compareArray(ActualUID, Usuario1)) {
        Serial.println("Juan Riquelme, acceso concedido");
        abrirCerrarPuerta();
      } else if (compareArray(ActualUID, Usuario2)) {
        Serial.println("María García, acceso concedido...");
        abrirCerrarPuerta();
      } else if (compareArray(ActualUID, Usuario3)) {
        Serial.println("Manuel Erices, acceso concedido...");
        abrirCerrarPuerta();
      } else {
        Serial.println("Acceso denegado...");
      }

      mfrc522.PICC_HaltA();
    }
  }

  // Verificar si la posición actual del servo ha sido cambiada
  int posicionActual = servoMotor.read();
  if (posicionActual != posicionEsperada) {
    Serial.println("¡Advertencia: Intento de forzar la puerta!");
    posicionEsperada = posicionActual; // Actualizar la posición esperada
  }
}

//Función para comparar dos vectores
boolean compareArray(byte array1[], byte array2[]) {
  for (byte i = 0; i < 4; i++) {
    if (array1[i] != array2[i]) return false;
  }
  return true;
}

//Función para abrir y cerrar la puerta con el servo
void abrirCerrarPuerta() {
  servoMotor.write(90, 50, true);  // Mueve el servomotor a 90° con velocidad de 30
  delay(2000);                     // Espera 2 segundos
  servoMotor.write(0, 30, true);   // Regresa el servo a la posición 0° con velocidad de 30
  posicionEsperada = 0;            // Actualiza la posición esperada a 0°
}
