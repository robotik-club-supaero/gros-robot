// #include <ArduinoJson.h>
// #include <elapsedMillis.h>

// // pour envoyer à fréquence fixe le json
// elapsedMillis sendTimer;
// const uint32_t SEND_INTERVAL_MS = 5; // Envoi tous les 5 ms (soit 200 Hz)
// // Un envoi si le buffer n'est pas plein prend 27-28 micro secondes
// // le buffer entier mets 62 ms à se déverser d'où l'utilité de l'envoi à fréquence fixe

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

// void setup() {
//   Serial.begin(500000);
//   pinMode(pinPot1,INPUT);
//   pinMode(pinPot2,INPUT);
// }

// void loop() {
//   // si la raspi a quelque chose à nous dire, on l'écoute
//   if (Serial.available() > 0) {
//     JsonDocument incomingDoc;
//     DeserializationError error = deserializeJson(incomingDoc, Serial);
    
//     if (!error) {
//       const char* cmd = incomingDoc["cmd"];
      
//       if (strcmp(cmd, "rotate") == 0) {
//         float angle = incomingDoc["angle"];
//       } 
//       else if (strcmp(cmd, "line") == 0) {
//         float x = incomingDoc["x"];
//         float y = incomingDoc["y"];
//       }
//     }
//   }

//   // remplissage de la struct
//   float valPot1 = map(analogRead(pinPot1),0,1023,0,360);
//   float valPot2 = map(analogRead(pinPot2),0,1023,0,360);

//   GpsData data = {valPot1,valPot2};

//   RobotState state = { millis(), 22.4f, data};
//   send_json_loop()
// }

// void send_json_loop() {
//   // on envoie le json que quand il faut. 
//   if (sendTimer >= SEND_INTERVAL_MS) {
//     sendTimer = 0; // Reset du timer
    
//     // création du json
//     JsonDocument doc;
//     doc["time"] = state.timestamp;
//     doc["batt"] = state.battery_voltage;
    
//     // Sous-objets imbriqués
//     doc["gps"]["lat"] = state.gps.lat;
//     doc["gps"]["lon"] = state.gps.lon;

//     serializeJson(doc, Serial);
//     Serial.println();
//   }
// }