#include <Arduino.h>
#include "odom.hpp"
#include "constantes.hpp"
#include <elapsedMillis.h>

elapsedMillis printTimer; // pour print quand on le veut.

void setup() {
  // Serial to PC -> TODO : à supprimer à terme car inutile
  while (!Serial) ; // wait for Serial Monitor to open
  Serial.println("Serial Monitor ready!");
  odom_setup();
}

void loop() {
  bool print_wanted = false;
  if (printTimer >= PRINT_INTERVAL_MS) {
    print_wanted = true; // on imprime à ce passage de boucle.
    printTimer = 0; // Reset du timer
  }
  
  odom_loop(print_wanted);
}