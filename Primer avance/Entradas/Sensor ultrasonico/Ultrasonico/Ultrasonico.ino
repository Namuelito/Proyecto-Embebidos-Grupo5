// Modificado para registrar eventos cuando la distancia es 10 cm o menos

#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);
}

void loop() {
    // Generar pulso de trigger
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Leer el pulso del pin ECHO
    long duration = pulseIn(ECHO_PIN, HIGH);

    // Calcular distancia en cm
    float distance = duration * 0.034 / 2;

    // Registrar evento si la distancia es 10 cm o menos
    if (distance <= 10) {
        Serial.println("Evento detectado: algo se encuentra a 10 cm o menos.");
        delay(1000); // Pausa para evitar registros continuos
    }
}