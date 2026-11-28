// ////////////////////////////////
// // Exemple de la librairie pour trouver la syntaxe des commandes odrive. 
// https://github.com/odriverobotics/ODrive/blob/master/Arduino/ODriveArduino/examples/ODriveArduinoTest/ODriveArduinoTest.ino
// ////////////////////////////////

#include <Arduino.h>
#include "cmd_odrive.hpp"


#include <ODriveUART.h>
#include <HardwareSerial.h> // Pour communiquer en UART avec l'Odrive

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
// On utilise le HardwareSerial Serial1 de la teensy pour communiquer avec l'odrive.
// Le HardwareSerial Serial0 est utilisé pour communiquer avec le PC.

// ODrive object
ODriveUART odrive(odrive_serial);

// Variable globale partagée initialisée une seule fois ici
Consigne consigne = {0,0,0,0,0,0};
Mesures mesures = {0,0,0,0,0,0,0};

// Toutes les variables définies en dehors de ces deux structures ne seront donc pas communiquées au gros robot
// Elles sont internes à l'asservissement dans la teensy.

void cmd_odrive_setup() {
  // ODrive uses 115200 baud
  odrive_serial.begin(115200);
  
  Serial.println("Waiting for ODrive...");
  while (odrive.getState() == AXIS_STATE_UNDEFINED) {
    delay(100);
  }
  Serial.println("found ODrive");
}


void cmd_odrive_machine_etats(bool en_marche, bool print_wanted) {
  if (en_marche) {
    if(print_wanted){Serial.println("Enabling closed loop control...");}  
    odrive_serial << "w axis0.requested_state " << AXIS_STATE_CLOSED_LOOP_CONTROL << "\n";
    odrive_serial << "w axis1.requested_state " << AXIS_STATE_CLOSED_LOOP_CONTROL << "\n";
    if(print_wanted){Serial.println("ODrive in closed loop!");}
  }
  else {
    if(print_wanted){Serial.println("Disabling closed loop control...");}  
    odrive_serial << "w axis0.requested_state " << AXIS_STATE_IDLE << "\n";
    odrive_serial << "w axis1.requested_state " << AXIS_STATE_IDLE << "\n";
    if(print_wanted){Serial.println("ODrive in idle!");}
  }

}

// void send_cons(Consigne cons){
// }

void send_speed(float tour_par_sec, int motor_num){
  // Protocole ASCII ODrive v3.6 : "v <axis> <velocity> <torque_ff>\n"
  if (motor_num == 0){
    odrive_serial << "v " << motor_num << " " << tour_par_sec << " 0\n";
  }
  else {
    odrive_serial << "v " << motor_num << " " << -tour_par_sec << " 0\n";
  }
  // note : j'ai mis le torque feed-forward à 0, pas besoin de feed-forward car pas de ressenti pilote, demander à Caro B.
}