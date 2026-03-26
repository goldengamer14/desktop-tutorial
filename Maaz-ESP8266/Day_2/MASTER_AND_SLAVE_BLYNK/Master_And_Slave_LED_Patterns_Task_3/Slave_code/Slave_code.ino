#include <ESP8266WiFi.h>
#include <espnow.h>

const uint8_t LED_PIN[3] = {D1, D2, D3};

typedef struct struct_message {
  int8_t ledState;
} struct_message;

struct_message incomingData;

int8_t currentMode = 0;  // store mode globally

// ---------------- PATTERNS ----------------

void bitwisePattern() {
  static int8_t mode = 0;

  digitalWrite(LED_PIN[0], mode & 1);
  digitalWrite(LED_PIN[1], (mode >> 1) & 1);
  digitalWrite(LED_PIN[2], (mode >> 2) & 1);

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

// ---------------- CALLBACK ----------------

void OnDataRecv(uint8_t * mac, uint8_t *incomingDataBytes, uint8_t len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  currentMode = incomingData.ledState;  // just store

  Serial.print("Mode changed to: ");
  Serial.println(currentMode);
}

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(115200);

  for (int8_t i = 0; i < 3; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    digitalWrite(LED_PIN[i], LOW);
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

// ---------------- LOOP ----------------

void loop() {
  switch (currentMode) {
    case 1:
      bitwisePattern();
      break;

    case 2:
      oneByOne();
      break;

    case 4:
      twoAtOnce();
      break;

    default:
      // optional: turn all OFF
      for(int i = 0; i < 3; i++)
        digitalWrite(LED_PIN[i], LOW);
      break;
  }
}