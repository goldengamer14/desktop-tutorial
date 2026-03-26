#include <ESP8266WiFi.h>
#include <espnow.h>

// #define LED_PIN D1  // GPIO5
const uint8_t LED_PIN[3] = {D1, D2, D3};

// Structure to receive data
typedef struct struct_message {
  int8_t ledState;
} struct_message;

struct_message incomingData;

void bitwisePattern() {
  static int8_t mode = 0;

  // for(int8_t i = 0; i < 3; i++)
  //   digitalWrite(LED_PIN[i], LOW);

  digitalWrite(LED_PIN[0], mode & 1);
  digitalWrite(LED_PIN[1], mode & 2);
  digitalWrite(LED_PIN[2], mode & 4);

  mode = (mode + 1) & 7;

  delay(1000);
}

void oneByOne() {
  static int8_t mode = 0;

  for(int8_t i = 0; i < 3; i++)
    digitalWrite(LED_PIN[i], LOW);

  digitalWrite(LED_PIN[mode], HIGH);

  mode = (mode + 1) % 3;

  delay(700);
}

void twoAtOnce() {
  static int8_t mode = 0;

  for(int8_t i = 0; i < 3; i++)
    digitalWrite(LED_PIN[i], HIGH);

  digitalWrite(LED_PIN[mode], LOW);

  mode = (mode + 1) % 3;

  delay(700);
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingDataBytes, uint8_t len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  Serial.print("LED State: ");
  Serial.println(incomingData.ledState);

  switch (incomingData.ledState) {
    case 1:
      bitwisePattern();
      break;

    case 2:
      oneByOne();

    case 4:
      twoAtOnce();
  }
}

void setup() {
  Serial.begin(115200);

  for (int8_t i = 0; i < 3; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    digitalWrite(LED_PIN[i], LOW);
  }

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