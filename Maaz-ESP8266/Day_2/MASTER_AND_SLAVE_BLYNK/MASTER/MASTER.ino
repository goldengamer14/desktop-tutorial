#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3VRqGSbBJ"
#define BLYNK_TEMPLATE_NAME "IOT Avishkar"
#define BLYNK_AUTH_TOKEN "5JvsHRKCWGX9XVqkdrolwLr-XrWMfRWB"

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "Redmi";
char pass[] = "12345679";

// Slave MAC
uint8_t slaveAddress[] = {0x50, 0x02, 0x91, 0xFC, 0x08, 0xB6};
// uint8_t defines integer only 
// Data structure
typedef struct struct_message {
  int ledState;
} struct_message;

struct_message dataToSend;

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

  // Add slave
  esp_now_add_peer(slaveAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// Button widget (V0)
BLYNK_WRITE(V0) {
  int value = param.asInt();

  dataToSend.ledState = value;

  esp_now_send(slaveAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));

  Serial.print("Sent to Slave: ");
  Serial.println(value);
}

void loop() {
  Blynk.run();
}