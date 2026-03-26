#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3RVjKCIPZ"
#define BLYNK_TEMPLATE_NAME "Brainrot"
#define BLYNK_AUTH_TOKEN "wm5WYvxgrTdYw1sOVZbltjf0ZmGoJwgT"

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Redmi";
char pass[] = "12345670";

uint8_t slaveAddress[] = {0x50, 0x02, 0x91, 0xFC, 0x08, 0xB6};

typedef struct struct_message {
  int8_t ledState;
} struct_message;

struct_message dataToSend;

// ✅ Send status callback
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_register_send_cb(OnDataSent);  // ✅ register callback

  int channel = WiFi.channel();
  Serial.print("WiFi Channel: ");
  Serial.println(channel);

esp_now_add_peer(slaveAddress, ESP_NOW_ROLE_SLAVE, channel, NULL, 0);

  Blynk.config(BLYNK_AUTH_TOKEN);

  if (Blynk.connect(5000)) {
    Serial.println("Blynk Connected");
  } else {
    Serial.println("Blynk NOT Connected");
  }
}

// -------- BLYNK BUTTONS --------

BLYNK_WRITE(V0) {
  int value = param.asInt();

  Serial.print("Sent to Slave, value = ");
  Serial.println(value);

  if (value == 1) {   // ✅ only when pressed
    dataToSend.ledState = 1;
    // dataToSend.ledState = value;
    esp_now_send(slaveAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

BLYNK_WRITE(V1) {
  int value = param.asInt();

  Serial.print("Sent to Slave, value = ");
  Serial.println(value);

  if (value == 1) {
    dataToSend.ledState = 2;
    esp_now_send(slaveAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

BLYNK_WRITE(V2) {
  int value = param.asInt();

  Serial.print("Sent to Slave, value = ");
  Serial.println(value);

  if (value == 1) {
    dataToSend.ledState = 4;
    esp_now_send(slaveAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

void loop() {
  Blynk.run();
}