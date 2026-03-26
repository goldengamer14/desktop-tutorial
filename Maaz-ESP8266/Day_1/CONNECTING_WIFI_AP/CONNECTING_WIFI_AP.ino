#include <ESP8266WiFi.h>

const char* ssid = "Iot_sanghatna";
const char* pass = "password";
void setup()
{
  // Serial.softAP(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass, 3, 0, 5);
}
// 1 here is for Number of channels, 0 stands for number of hidden channels, 5 is her to set the device connection limit   
void loop()
{
  // Serial.println(WiFi.localIP());
}