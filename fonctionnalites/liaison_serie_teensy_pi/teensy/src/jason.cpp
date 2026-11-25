#include <Arduino.h>

#include "jason.hpp"
#include "constantes.hpp"

#include <ArduinoJson.h>
#include <elapsedMillis.h>

// On initialise les structures partagées (osef de la valeur des champs)
GpsData data= {0.0,0.0};
RobotState state= {0, 22.4f, data};
Command commande = {false, 10.0, 0,0};

// variables internes au programme :
elapsedMillis send_json_timer; // pour envoyer à fréquence fixe le json

void receive_jason_loop(){
  if (Serial.available() > 0) {
    JsonDocument incomingDoc;
    DeserializationError error = deserializeJson(incomingDoc, Serial);
    
    if (!error) {
      const char* cmd = incomingDoc["cmd"];
      
      if (strcmp(cmd, "rotate") == 0) {
        commande.line=false;
        commande.angle = incomingDoc["angle"];
        commande.x = 0;
        commande.y = 0;
      } 
      else if (strcmp(cmd, "line") == 0) {
        commande.line=true;
        commande.angle = 0;
        commande.x = incomingDoc["x"];
        commande.y = incomingDoc["y"];
      }
    }
  }
}

void send_jason_loop() {
  // on envoie le json que quand il faut. 
  if (send_json_timer >= JSON_INTERVAL_MS) {
    send_json_timer = 0; // Reset du timer
    
    // création du json
    JsonDocument doc;
    doc["time"] = state.timestamp;
    doc["batt"] = state.battery_voltage;
    
    // Sous-objets imbriqués
    doc["gps"]["lat"] = state.gps.lat;
    doc["gps"]["lon"] = state.gps.lon;
    doc["cmd"]["line"] = commande.line;
    doc["cmd"]["angle"] = commande.angle;
    doc["cmd"]["x"] = commande.x;
    doc["cmd"]["y"] = commande.y;

    serializeJson(doc, Serial);
    Serial.println();
  }
}
