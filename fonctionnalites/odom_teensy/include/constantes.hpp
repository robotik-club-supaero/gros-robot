#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

// pour le main
constexpr int PRINT_INTERVAL_MS = 200; //valable pour tous les bouts de code !

// Utilitaires
constexpr float DEG_VERS_RAD = M_PI / 180.0;
constexpr float RAD_VERS_DEG = 180.0 / M_PI;

// Odométrie du robot
// regarder la doc pour les régler. Les formules approximatives sont données ici. 
constexpr float TICKS_PAR_CM = 514.9039070749735;   // Formule : résolution encodeur / (pi * diamètre roue codeuse)
constexpr float ENTRAXE_CM   = 11.35;  // Distance entre les 2 roues codeuses (en cm)
constexpr float RATIO_G_D    = 1.0020293330873535;    // A ajuster, permet de prendre en compte qu'une roue soit un tout petit peu plus grande qu'une autre

#endif