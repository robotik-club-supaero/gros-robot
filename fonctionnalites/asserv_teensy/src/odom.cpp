#include <Arduino.h>
#include "odom.hpp"

// inclusions privées nécessaires aux détails d'implémentation
#include <Encoder.h>
#include "pins.hpp"
#include "constantes.hpp"

// initialisation des variables extern (= partagées) de position
// Elles sont itinialisées seulement ici !
float x = 0.0;      // en cm, extern
float y = 0.0;      // en cm, extern
float theta = 0.0;  // en degrés, extern

// variables globales au fichier, non partagées au reste des programmes
float distG_tot = 0.0; // en cm, variable globale
float distD_tot = 0.0; // en cm, variable globale

long ancienTicksG = 0; // globale
long ancienTicksD = 0; // globale


/*Adapté de : 
https://www.pjrc.com/teensy/td_libs_Encoder.html

Les 2 signals de chaque encodeurs sont connectés à des pins qui ont la capacité d'interrupt car tous les pins DIGITAUX
de la Teensy 4.1 ont la capacité.

Il est dit dans la doc de ne pas utiliser softwareSerial car ça peut poser problème. pk ? partie Interrupt Latency Requirements
*/ 



Encoder encGauche(encodeur_g_pin1, encodeur_g_pin2);
Encoder encDroit(encodeur_d_pin1, encodeur_d_pin2);


void odom_setup() {
  // rien à faire
}


void odom_loop(bool print_wanted) {
  long ticksG = -encGauche.read();
  long ticksD = encDroit.read();

  // positif si le robot avance, négatif s'il recule
  long deltaTicksG = ticksG - ancienTicksG; 
  long deltaTicksD = ticksD - ancienTicksD;

  // Conversion en millimètres parcourus sur l'élément de parcours dl
  float distG = (float)deltaTicksG / TICKS_PAR_CM;
  float distD = ((float)deltaTicksD * RATIO_G_D) / TICKS_PAR_CM;
  
  // Chemin total parcouru par chaque roue
  distG_tot += distG;
  distD_tot += distD;

  // Calcul du déplacement linéaire et angulaire sur l'élément de parcours dl
  float deltaDistance = (distD + distG) / 2.0;
  float deltaTheta = RAD_TO_DEG * (distD - distG) / ENTRAXE_CM;

  // Mise à jour de la position.
  // Hypothèse sous jacente : 
  // On suppose que pendant un très court instant, le robot avance tout droit selon son orientation actuelle (theta),
  // puis tourne sur lui-même de deltaTheta. D'où le fait qu'on fasse les calculs avec l'ancien theta. 
  
  x     += deltaDistance * cos(theta*DEG_TO_RAD);
  y     += deltaDistance * sin(theta*DEG_TO_RAD);
  theta += deltaTheta;

  // Optionnel : garder theta entre -180 et +180
  while (theta > 180.0)  theta -= 360.0;
  while (theta < -180.0) theta += 360.0;

  if (print_wanted) {
    // Affichage pour debug
    Serial.print("\t distG_tot (cm): "); Serial.print(distG_tot);
    Serial.print("\t distD_tot (cm): "); Serial.print(distD_tot);
    Serial.print("\t x (cm): "); Serial.print(x);
    Serial.print("\t y (cm): "); Serial.print(y);
    Serial.print("\t theta (deg): "); Serial.print(theta);
    Serial.println();
  }

  // on mets les anciens ticks à ceux courants pour la boucle suivante
  ancienTicksG = ticksG;
  ancienTicksD = ticksD;
}