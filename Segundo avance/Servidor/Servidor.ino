#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Pines del botón y LEDs
const int btnPin = 0;  // Botón conectado al pin 0
const int led1Pin = 2; // LED1 en el pin 2
const int led2Pin = 22; // LED2 en el pin 22

// Configuración WiFi
const char* ssid = "HECTOR_2.4";
const char* password = "HECTOR2295874";

// URL del servidor
const char* serverUrl = "http://192.168.1.13/urequestESP32/urequestPHP.php";

WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(btnPin, INPUT_PULLUP); // Configurar botón con pull-up interno
  pinMode(led1Pin, OUTPUT);      // Configurar LED1 como salida
  pinMode(led2Pin, OUTPUT);      // Configurar LED2 como salida

  Serial.begin(115200);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
}

void handleSubmit(int led1State, int led2State) {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverUrl);

    // Crear JSON con el estado de los LEDs
    StaticJsonDocument<200> jsonData;
    jsonData["led1on"] = led1State;
    jsonData["led2on"] = led2State;

    String requestBody;
    serializeJson(jsonData, requestBody);

    // Enviar solicitud POST
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(requestBody);

    // Leer respuesta
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(response);
    } else {
      Serial.print("Error al enviar datos: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Liberar recursos
  } else {
    Serial.println("WiFi desconectado. No se pudo enviar datos.");
  }
}

void loop() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(btnPin);

  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(200); // Evitar rebotes

    // Cambiar estado de los LEDs aleatoriamente
    int led1State = random(0, 2); // 0 o 1
    int led2State = random(0, 2);

    digitalWrite(led1Pin, led1State);
    digitalWrite(led2Pin, led2State);

    // Enviar datos al servidor
    handleSubmit(led1State, led2State);
  }
  lastButtonState = currentButtonState;
}
