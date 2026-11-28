#ifndef CMD_ODRIVE_HPP
#define CMD_ODRIVE_HPP

// le haut niveau demande d'exécuter un mouvement selon la rampe spécifiée en Input. 
// s'il ne veut plus qu'on bouge, c'est un distance = 0 et angle = 0. La valeur des autres champs importe peu alors. 
// On s'impose de ne jamais avoir distance != 0 en même temps que angle != 0. 
struct Consigne{
  float distance;           // distance à avancer en [cm]
  float angle;              // angle à atteindre en [degrés]
  float consigne_vit_lin;   // consigne de vitesse linéaire en [cm/s]
  float consigne_vit_rot;   // consigne de vitesse de rotation en [degrés/s]
  float consigne_accel_lin; // consigne de accélération linéaire en [cm/s²]
  float consigne_accel_rot; // consigne de accélération de rotation en [degrés/s²]
};
// toutes les données sont en float car c'est ce qu'utilise la odrive. 

extern Consigne consigne;
// la structure est définie ici et initialisée dans le cpp correspondant

// On renvoie au haut niveau les données (brutes, précises) actuelles du robot. 
// Le haut niveau se charge de décider si la position demandée a été atteinte suffisamment précisément ou non.
// En effet, cela dépend du contexte (approche précise d'un objet, ou traversée de table).
struct Mesures{
  float position;
  float angle;
  float vit_lin;    // vitesse linéaire
  float vit_rot;    // vitesse de rotation
  float accel_lin;  // accélération linéaire
  float accel_rot;  // accélération de rotation
  float timestamp;
};

extern Mesures mesures;
// initialisé dans le cpp

void cmd_odrive_setup(void);
void cmd_odrive_machine_etats(bool en_marche, bool print_wanted);

// todo, supprimer de l'interface publique
void send_speed(float tour_par_sec, int motor_num);

#endif