# Mémo : comment déclarer proprement un .hpp

Un fichier `.hpp` sert de contrat avec l'EXTERIEUR : il indique *ce qui existe* (types, constantes, signatures de fonctions, variables globales) pour les AUTRES bouts de code, en **CACHANT l'intérieur de l'implémentation du service (important !)**.

## `.hpp` typique
```c
#ifndef MON_FICHIER_HPP
#define MON_FICHIER_HPP

// Les constantes (Pins, constantes, etc). Remplace le #define en permettant de mettre un type : mieux. 
constexpr float GAIN_PID=1.67;
constexpr int PIN_BOUTON = 7;
constexpr int PIN_POTENTIOMETRE = A2;

// Librairies minimales utilisées par le code cpp associé
#include <Librairie.h>

// Déclarations des structs dont auront besoin les autres bouts de code pour intéragir avec ce bout de code
struct MaStruct {
  float temperature;
  float humidite;
  bool valide;
};
// remarque : la manière de définir des structs en C++ est différente du C. Pas besoin du typedef. 

// les structs nécessaires à l'implémentation interne sont déclarées dans le .cpp et cachées au reste de la base de code. 


// Signatures des fonctions dont auront besoin les autres bouts de code pour intéragir avec ce bout de code
void capteur_setup(void);
ma_struct capteur_lire(void);

// idem, les fonctions nécessaires à l'implémentation interne resteront cachées dans le .cpp !

// Si utilisation de variables globales partagées avec d'autres fichier
extern int compteur_global;
extern volatile bool drapeau_interruption; // volatile car interruption

// Les variables globales au fichier restent cachées dans le .cpp car détail d'implémentation. 

#endif
```
## Autres conseils :

`#include <Arduino.h>` doit être mis dans tous les fichiers cpp. Si le fichier .h en a besoin pour un type défini par Arduino, le mettre. 

ATTENTION, on ne met jamais la **définition = initialisation** d'une variable globale. Une variable globale (`extern`) doit être définie = initialisée dans un seul fichier cpp. 

Les variables globales au fichier, non partagées avec d'autres fichiers, sont des détails de l'implémentation du service. 
Elles n'apparaissent donc pas aux yeux des autres, ie dans le .hpp. 
On les déclare dans le .cpp en début de fichier en dehors de toute fonction. 

Eviter les inclusions circulaires...

## Règles dans les .cpp
Aucunes constantes dans les .cpp, toutes rassemblées dans constantes.hpp !

Les variables déclarées en `extern` dans mon_fichier.hpp doivent être initialisées dans mon_fichier.cpp. C'est logique, tout ce que gère un service est centralisé par ce service ! UNE SEULE INITIALISATION sinon compilateur pas content =(.