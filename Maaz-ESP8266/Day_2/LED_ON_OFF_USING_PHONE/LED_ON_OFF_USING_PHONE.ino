#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); 
const int ledPin = 5; 

void handleRoot() {
  String html = "<h1>Traditional Control</h1>";
  html += "<a href='/toggle'><button>PRESS TO TOGGLE</button></a>";
  server.send(200, "text/html", html); 
}

void handleToggle() {
  digitalWrite(ledPin, !digitalRead(ledPin)); 
  server.sendHeader("Location", "/");          
  server.send(303); 
}

void setup() {
  pinMode(ledPin, OUTPUT); 
  WiFi.softAP("Iot_Sanghatna", "password",1,0,3); 

  server.on("/", handleRoot); 
  server.on("/toggle", handleToggle);
  
  server.begin();
}

void loop() {
  server.handleClient(); 
}