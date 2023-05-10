#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Net Name";          // inserisci il nome della tua rete wi-fi
const char* password = "pwd";  // inserisci la password della tua rete wi-fi

 #define RELAY 0 // relay connected to  GPIO0

ESP8266WebServer server(80);

void setup() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);

  Serial.begin(115200);

  // connessione alla rete wi-fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connessione in corso...");
  }

  Serial.println("connessione wi-fi stabilita");
  Serial.print("indirizzo ip: ");
  Serial.println(WiFi.localIP());

  server.on("/comando", handlecomando);
  server.begin();
  Serial.println("server avviato");
}

void loop() {
  server.handleClient();
}

void handlecomando() {
  String comando = server.arg("comando");

  if (comando == "accendi") {
    digitalWrite(RELAY, LOW);
    server.send(200, "text/plain", "led acceso");
  } else if (comando == "spegni") {
    digitalWrite(RELAY, HIGH);
    server.send(200, "text/plain", "led spento");
  } else {
    server.send(400, "text/plain", "comando non valido");
  }
}
