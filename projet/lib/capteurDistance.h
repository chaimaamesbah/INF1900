/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier contient l'implémentation des méthodes de la classe `CapteurDistance`. 
Cette classe gère les fonctionnalités associées à un capteur de distance, telles que la lecture 
de la fréquence en fonction de la distance détectée, la gestion des LEDs, et la détection 
de la présence d'obstacles à des distances spécifiques.

- CapteurDistance(can *can, Del *del) : Constructeur qui initialise le capteur de distance et configure la broche d'entrée.
- uint8_t obtenirFrequence() : Lit et retourne la fréquence mesurée par le capteur.
- void afficherEtGererLeds() : Affiche la valeur mesurée par le capteur via l'USART et gère les LEDs en fonction de la lecture.
- bool estPoteauDetecteDixCm() : Vérifie si un poteau est détecté à une distance de 10 cm.
- bool estPoteauDetecteTrenteCm() : Vérifie si un poteau est détecté à une distance de 30 cm.

Identifications des matérielles : 
- Input :
    - Broche associée au capteur de distance sur le port A (PA0).
- Output :
    - Aucune sortie directe, mais gestion indirecte des LEDs via la classe `Del`
    - Transmission des données via l'USART
*/


#ifndef CAPTEUR_DISTANCE_H
#define CAPTEUR_DISTANCE_H

#define F_CPU 8000000UL
#include "can.h"
#include "del.h"
#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>

class CapteurDistance {
public:
    CapteurDistance(can* can, Del* del);

    uint8_t obtenirFrequence();
    void afficherEtGererLeds();
    bool estPoteauDetecteDixCm();
    bool estPoteauDetecteTrenteCm();

private:
    can* can_;
    Del* del_;

    const uint8_t pinCapteur_ = PA0;
    const uint8_t frequencePoteauDixCmMinimum_ = 100;
    const uint8_t frequencePoteauDixCmMaximum_ = 105;
    const uint8_t frequencePoteauTrenteCmMaximum_ = 27;

    char buffer[20];
    uint8_t frequenceLectureCapteur_;
    bool estPoteauDetecte_;
};

#endif