#include <ESP8266WiFi.h>

const char *ssid = "Dipak";
const char *pass = "0987654321";
const char *ssidap = "Iot_sanghatna";
const char *passap = "password";
void setup()
{
  //  Serial.begin(115200);
  //  Serial.softAP(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidap, passap, 1, 0, 5);
  WiFi.begin(ssid, pass);
}
// 1 here is for Number of channels, 0 stands for number of hidden channels, 5 is here to set the device connection limit
void loop()
{
  // Serial.println(WiFi.localIP());
}