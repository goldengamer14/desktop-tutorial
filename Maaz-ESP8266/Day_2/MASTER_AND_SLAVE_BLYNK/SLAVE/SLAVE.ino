#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED_PIN D1  // GPIO5


// Structure to receive data
typedef struct struct_message {
  int ledState;
} struct_message;

struct_message incomingData;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingDataBytes, uint8_t len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  Serial.print("LED State: ");
  Serial.println(incomingData.ledState);

  digitalWrite(LED_PIN, incomingData.ledState);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA); // Required for ESP-NOW

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}