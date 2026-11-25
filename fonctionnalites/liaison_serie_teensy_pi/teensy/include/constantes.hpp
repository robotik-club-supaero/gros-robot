#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

constexpr int JSON_INTERVAL_MS = 5; // fréquence d'envoi-réception du json en Hz. 
// Envoi tous les 5 ms (soit 200 Hz)
// Un envoi si le buffer n'est pas plein prend 27-28 micro secondes
// le buffer entier mets 62 ms à se déverser d'où l'utilité de l'envoi à fréquence fixe


#endif