#include <ESP8266WiFi.h>

const char* ssid = "Nothing";
const char* pass = "password";
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, pass);
}
// 1 here is for Number of channels, 0 stands for number of hidden channels, 5 is her to set the device connection limit   
void loop()
{
  Serial.println(WiFi.localIP());
}