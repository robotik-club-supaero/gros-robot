// #include "core_pins.h"
// #include "pins_arduino.h"
// #include <ArduinoJson.h>
// #include <elapsedMillis.h>


// struct GpsData {
//   float lat;
//   float lon;
// };

// struct RobotState {
//   uint32_t timestamp;
//   float battery_voltage;
//   GpsData gps; // Structure imbriquée
// };

// const int pinPot1 = A17;
// const int pinPot2 = A16;

// // variables pour le benchmark, calcule le temps min/max pour envoyer le json sur 1000 boucles
// uint32_t max_duration = 0;
// uint32_t min_duration = 0xFFFFFFFF;
// uint32_t counter = 0;

// // pour envoyer à fréquence fixe le json
// elapsedMillis sendTimer;
// const uint32_t SEND_INTERVAL_MS = 5; // Envoi tous les 5 ms (soit 200 Hz)
// // à savoir qu'un envoi si le buffer n'est pas plein prend 27-28 micro secondes
// // le buffer entier mets 62 ms à se déverser d'où l'utilité de l'envoi à fréquence fixe


// void setup() {
//   Serial.begin(500000);
//   pinMode(pinPot1,INPUT);
//   pinMode(pinPot2,INPUT);
// }

// void loop() {
//   // remplissage de la struct
//   float valPot1 = map(analogRead(pinPot1),0,1023,0,360);
//   float valPot2 = map(analogRead(pinPot2),0,1023,0,360);

//   GpsData data = {valPot1,valPot2};

//   RobotState state = { millis(), 22.4f, data};

//   // on envoie le json que quand il faut. 
//   if (sendTimer >= SEND_INTERVAL_MS) {
//     sendTimer = 0; // Reset du timer
    
//     // début du timer pour faire le benchmark de l'envoi du json
//     uint32_t start = micros();

//     // création du json
//     JsonDocument doc;
//     doc["time"] = state.timestamp;
//     doc["batt"] = state.battery_voltage;
    
//     // Sous-objets imbriqués
//     doc["gps"]["lat"] = state.gps.lat;
//     doc["gps"]["lon"] = state.gps.lon;
//     doc["stats"]["min"] = min_duration;
//     doc["stats"]["max"] = max_duration;

//     serializeJson(doc, Serial);
//     Serial.println();
    
//     uint32_t elapsed = micros() - start;
//     if (elapsed > max_duration) max_duration = elapsed;
//     if (elapsed < min_duration) min_duration = elapsed;
//     counter++;

//     if (counter >= 1000) { // Toutes les 1000 boucles, on remet à 0 les stats
//       max_duration = 0;
//       min_duration = 0xFFFFFFFF;
//       counter = 0;
//     }
//   }
// }
