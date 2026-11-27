// ////////////////////////////////
// // Exemple de la librairie pour trouver la syntaxe des commandes odrive. 
// https://github.com/odriverobotics/ODrive/blob/master/Arduino/ODriveArduino/examples/ODriveArduinoTest/ODriveArduinoTest.ino
// ////////////////////////////////

#include <Arduino.h>
#include "cmd_odrive.hpp"


#include <ODriveArduino.h>
#include <HardwareSerial.h> // Pour communiquer en UART avec l'Odrive
#include <SoftwareSerial.h>

// Les 2 lignes qui suivent sont issues de l'exemple de la librairie.
// Elles permettent d'écrire les commandes à envoyer à la odrve facilement et en une seule ligne.
// Avec par exemple : Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
// Donc ne pas y toucher...

// Teensy 3 and 4 (all versions) - Serial1
// pin 0: RX - connect to ODrive TX
// pin 1: TX - connect to ODrive RX
// See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
HardwareSerial& odrive_serial = Serial1;

// ODrive object
ODriveArduino odrive(odrive_serial);

// Variable globale partagée initialisée une seule fois ici
Consigne consigne = {0,0,0,0,0,0};
Mesures mesures = {0,0,0,0,0,0,millis()};

// Toutes les variables définies en dehors de ces deux structures ne seront donc pas communiquées au gros robot
// Elles sont internes à l'asservissement dans la teensy.

void cmd_odrive_setup() {
  // ODrive uses 115200 baud
  odrive_serial.begin(115200);

  // // Run calibration sequence
  // if (c == '0' || c == '1') {
  //   int motornum = c-'0';
  //   int requested_state;

  //   requested_state = AXIS_STATE_MOTOR_CALIBRATION;
  //   Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
  //   if(!odrive.run_state(motornum, requested_state, true)) return;

  //   requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
  //   Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
  //   if(!odrive.run_state(motornum, requested_state, true, 25.0f)) return;

  //   requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL;
  //   Serial << "Axis" << c << ": Requesting state " << requested_state << '\n';
  //   if(!odrive.run_state(motornum, requested_state, false /*don't wait*/)) return;
  // }
}


// void cmd_odrive_machine_etats(bool en_marche, bool print_wanted) {
//   if (en_marche) {

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
//   else {
//     if(print_wanted){
//       Serial.println("à l'arrêt");
//     }
//   }
// 
// }

// void send_cons(Consigne cons){
// }

void send_speed(float tour_par_sec, int motor_num){
  odrive_serial << "v " << motor_num << " " << tour_par_sec << " 0\n";
  // note : j'ai mis le torque feed-forward à 0, pas besoin de feed-forward car pas de ressenti pilote, demande à Caro B.
}