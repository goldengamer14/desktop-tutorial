#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); 
const int ledPin = D1; 

void handleRoot() {
  String html = "\
  <a href=\"/toggle\">\
  <button id=\"btn\">Turn On</button>\
  </a>\
\
    <script>\
        const btn = document.getElementById(\"btn\");\
        let on = true;\
\
        btn.addEventListener(\"click\", () => {\
            if (on) {\
                on = false;\
                btn.style.backgroundColor = \"gray\";\
                btn.innerHTML = \"Turn Off\";\
            } else {\
                on = true;\
                btn.style.backgroundColor = \"blue\";\
                btn.innerHTML = \"Turn On\";\
            }\
        })\
    </script>\
    \
     <style>\
        body {\
            background-color: #444;\
            height: 100vh;\
            \
            display: flex;\
            justify-content: center;\
            align-items: center;\
        }\
        #btn {\
            background-color: blue;\
            color: white;\
            border: 3px solid red;\
\
            border-radius: 1030px;\
            width: 5rem;\
            height: 5rem;\
        }\
\
        #btn:hover {\
            cursor: pointer;\
        }\
    </style>\
    ";
  server.send(200, "text/html", html); 
}
// 200 means website is okay, 404 means website not found, 303 means redirection / see other,  

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