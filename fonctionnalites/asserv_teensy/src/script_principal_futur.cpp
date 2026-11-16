// ////////////////////////////////
// // Exemple de la librairie pour trouver la syntaxe des commandes odrive. 
// https://github.com/odriverobotics/ODrive/blob/master/Arduino/ODriveArduino/examples/ODriveArduinoTest/ODriveArduinoTest.ino
// ////////////////////////////////


// // includes classiques
// #include <Arduino.h>
// #include <ArduinoJson.h> // pour la communication en JSON
// #include <elapsedMillis.h> // pour la communication en JSON 
// #include <HardwareSerial.h> // Pour communiquer en UART avec l'Odrive
// #include <SoftwareSerial.h> // Pour pouvoir utiliser le moniteur série de la teensy sur le PC. TODO : à supprimer à terme.
// #include <ODriveArduino.h>

// // Les 2 lignes qui suivent sont issues de l'exemple de la librairie.
// // Elles permettent d'écrire les commandes à envoyer à la odrve facilement et en une seule ligne.
// // Avec par exemple : Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
// template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
// template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
// // Donc ne pas y toucher...

// // Teensy 3 and 4 (all versions) - Serial1
// // pin 0: RX - connect to ODrive TX
// // pin 1: TX - connect to ODrive RX
// // See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
// HardwareSerial& odrive_serial = Serial1;

// // ODrive object
// ODriveArduino odrive(odrive_serial);

// // pour envoyer à fréquence fixe le json
// elapsedMillis sendTimer;
// const uint32_t SEND_INTERVAL_MS = 5; // Envoi tous les 5 ms (soit 200 Hz)
// // Un envoi si le buffer n'est pas plein prend 27-28 micro secondes
// // le buffer entier mets 62 ms à se déverser d'où l'utilité de l'envoi à fréquence fixe


// // le haut niveau demande d'exécuter un mouvement selon la rampe spécifiée en Input. 
// // s'il ne veut plus qu'on bouge, c'est un distance = 0 et angle = 0. La valeur des autres champs importe peu alors. 
// // On s'impose de ne jamais avoir distance != 0 en même temps que angle != 0. 
// typedef struct {
//   float consigne_vit_lin; // consigne de vitesse linéaire en [cm/s]
//   float consigne_vit_rot; // consigne de vitesse de rotation en [degrés/s]
//   float consigne_accel_lin; // consigne de accélération linéaire en [cm/s²]
//   float consigne_accel_rot; // consigne de accélération de rotation en [degrés/s²]
//   float distance; // distance à avancer en [cm]
//   float angle; // angle à atteindre en [degrés]
// } Input;
// // toutes les données sont en float car c'est ce qu'utilise la odrive. 


// // On renvoie au haut niveau les données (brutes, précises) actuelles du robot. 
// // Le haut niveau se charge de décider si la position demandée a été atteinte suffisamment précisément ou non.
// // En effet, cela dépend du contexte (approche précise d'un objet, ou traversée de table).
// typedef struct {
//   float position;
//   float angle;
//   float vit_lin; // vitesse linéaire
//   float vit_rot; // vitesse de rotation
//   float accel_lin; // accélération linéaire
//   float accel_rot; // accélération de rotation
//   float timestamp;
// } Output;

// // Toutes les variables définies en dehors de ces deux structures ne seront donc pas communiquées au gros robot
// // Elles sont internes à l'asservissement dans la teensy.

// void setup() {
//   // ODrive uses 115200 baud
//   odrive_serial.begin(115200);

//   // Serial to PC -> TODO : à supprimer à terme car inutile
//   Serial.begin(115200);
//   while (!Serial) ; // wait for Serial Monitor to open
//   Serial.println("ODriveArduino");
//   Serial.println("Ready!");
// }

// void loop() {
//   // si la raspi a quelque chose à nous dire, on l'écoute
//   if (Serial.available() > 0) {
//     JsonDocument incomingDoc;
//     DeserializationError error = deserializeJson(incomingDoc, Serial);
    
//     if (!error) {
//       const char* cmd = incomingDoc["cmd"];

//       if (strcmp(cmd, "rotation") == 0) {
//         float angle = incomingDoc["angle"];
//       } 
//       else if (strcmp(cmd, "ligne") == 0) {
//         float dist = incomingDoc["dist"];
//       }
//     }
//   }

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

//   if (Serial.available()) {
//     char c = Serial.read();

//     // Run calibration sequence
//     if (c == '0' || c == '1') {
//       int motornum = c-'0';
//       int requested_state;

//       requested_state = AXIS_STATE_MOTOR_CALIBRATION;
//       Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
//       if(!odrive.run_state(motornum, requested_state, true)) return;

//       requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
//       Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
//       if(!odrive.run_state(motornum, requested_state, true, 25.0f)) return;

//       requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL;
//       Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
//       if(!odrive.run_state(motornum, requested_state, false /*don't wait*/)) return;
//     }

//     // Sinusoidal test move
//     if (c == 's') {
//       Serial.println("Executing test move");
//       for (float ph = 0.0f; ph < 6.28318530718f; ph += 0.01f) {
//         float pos_m0 = 2.0f * cos(ph);
//         float pos_m1 = 2.0f * sin(ph);
//         odrive.SetPosition(0, pos_m0);
//         odrive.SetPosition(1, pos_m1);
//         delay(5);
//       }
//     }

//     // Read bus voltage
//     if (c == 'b') {
//       odrive_serial << "r vbus_voltage\n";
//       Serial << "Vbus voltage: " << odrive.readFloat() << '\n';
//     }

//     // print motor positions in a 10s loop
//     if (c == 'p') {
//       static const unsigned long duration = 10000;
//       unsigned long start = millis();
//       while(millis() - start < duration) {
//         for (int motor = 0; motor < 2; ++motor) {
//           Serial << odrive.GetPosition(motor) << '\t';
//         }
//         Serial << '\n';
//       }
//     }
//   }
// }

