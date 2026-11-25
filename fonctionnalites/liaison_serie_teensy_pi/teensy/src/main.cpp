#include <Arduino.h>
#include "jason.hpp"
#include "pins.hpp"

void setup() {
  Serial.begin(500000);
  pinMode(pinPot1,INPUT);
  pinMode(pinPot2,INPUT);
}

void loop() {
  // remplissage de la struct, pour l'exemple
  float valPot1 = map(analogRead(pinPot1),0,1023,0,360);
  float valPot2 = map(analogRead(pinPot2),0,1023,0,360);

  data = {valPot1,valPot2};

  state = { millis(), 22.4f, data};

  receive_jason_loop();
  send_jason_loop();
}