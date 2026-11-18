#include "pins.hpp"
#include "constantes.hpp"
#include <Arduino.h>
#include <Encoder.h>
#include <elapsedMillis.h>

/*Adapté de : 
https://www.pjrc.com/teensy/td_libs_Encoder.html

Les 2 signals de chaque encodeurs sont connectés à des pins qui ont la capacité d'interrupt car tous les pins DIGITAUX
de la Teensy 4.1 ont la capacité.

Il est dit dans la doc de ne pas utiliser softwareSerial car ça peut poser problème. pk ? partie Interrupt Latency Requirements
*/ 

elapsedMillis printTimer; // pour print 5 fois par seconde (même si on a une fréq d'acquisition bien + haute) sinon ça fait trop
const uint32_t PRINT_INTERVAL_MS = 200; // Envoi 5 fois par seconde


Encoder encGauche(encodeur_g_pin1, encodeur_g_pin2);
Encoder encDroit(encodeur_d_pin1, encodeur_d_pin2);

double x = 0.0;      // en mm
double y = 0.0;      // en mm
double distG_tot = 0.0; // en mm
double distD_tot = 0.0; // en mm
double theta = 0.0;  // en radians

long ancienTicksG = 0;
long ancienTicksD = 0;

void setup() {
  // Serial to PC -> TODO : à supprimer à terme car inutile
  while (!Serial) ; // wait for Serial Monitor to open
  Serial.println("Serial Monitor ready!");
}


void loop() {
  long ticksG = -encGauche.read();
  long ticksD = encDroit.read();

  // positif si le robot avance, négatif s'il recule
  long deltaTicksG = ticksG - ancienTicksG; 
  long deltaTicksD = ticksD - ancienTicksD;

  // Conversion en millimètres parcourus sur l'élément de parcours dl
  double distG = (double)deltaTicksG / TICKS_PAR_MM;
  double distD = ((double)deltaTicksD * RATIO_G_D) / TICKS_PAR_MM;
  
  // Chemin total parcouru par les 2 roues 
  distG_tot += distG;
  distD_tot += distD;
  // Pour régler le facteur gauche-droite, faire une ligne droite contre un bord de table.
  // Ajuster le facteur pour que distG_tot = distD_tot

  // Calcul du déplacement linéaire et angulaire sur l'élément de parcours dl
  double deltaDistance = (distD + distG) / 2.0;
  double deltaTheta = (distD - distG) / ENTRAXE_MM;

  // Mise à jour de la position.
  // Hypothèse sous jacente : 
  // On suppose que pendant un très court instant, le robot avance tout droit selon son orientation actuelle (theta),
  // puis tourne sur lui-même de deltaTheta.
  
  x     += deltaDistance * cos(theta);
  y     += deltaDistance * sin(theta);
  theta += deltaTheta;

  // Optionnel : garder theta entre -PI et +PI
  while (theta > M_PI)  theta -= 2.0 * M_PI;
  while (theta < -M_PI) theta += 2.0 * M_PI;

  // Affichage pour debug
  Serial.print("\t distG_tot (cm): "); Serial.print(distG_tot/10);
  Serial.print("\t distD_tot (cm): "); Serial.print(distD_tot/10);
  Serial.print("\t x (cm): "); Serial.print(x/10);
  Serial.print("\t y (cm): "); Serial.print(y/10);
  Serial.print("\t theta (deg): "); Serial.print(theta * 180.0 / M_PI);
  Serial.println();
  delay(200);

  // on mets les anciens ticks à ceux courants pour la boucle suivante
  ancienTicksG = ticksG;
  ancienTicksD = ticksD;
}