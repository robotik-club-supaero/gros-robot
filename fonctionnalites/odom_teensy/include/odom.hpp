#ifndef ODOM_HPP
#define ODOM_HPP

// La seule utilité de odom.cpp est d'updater x,y, theta en permanence
// odom.cpp n'expose donc que son setup et sa loop pour pouvoir être lancés depuis main.cpp
// En effet, en arduino, il ne doit y avoir qu'un setup() et un loop() parmi tous les programmes et c'est dans le main.cpp

void odom_setup(void);
void odom_loop(bool print_wanted);

// Repère de physique classique :
// x est à l'horizontale, y à la verticale quand on regarde la table pour qu'elle soit plus haute que large. 
extern float x; // (en cm) variable globale, initialisée dans odom.hpp
extern float y; // (en cm) variable globale, initialisée dans odom.hpp
extern float theta; // (en degres) variable globale, initialisée dans odom.hpp

// Remarque : c'est odom qui expose au monde le x,y,theta car c'est lui qui les connait et les initialise. 
// Libre au reste du code de les utiliser. 
// main.cpp n'est qu'un chef d'orchestre. Ce n'est pas à lui de gérer x,y,theta. 
#endif