/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Ce programme gère une minuterie sur un microcontrôleur AVR. 
Il permet de démarrer la minuterie avec un prescaler sélectionnable, de l'arrêter et de la réinitialiser pour gérer des délais précis.

Identifications des matérielles : 
- Input :
        - Aucune
- Output :
        - Minuterie interne du microcontrôleur

*/

#ifndef MINUTERIE_H
#define MINUTERIE_H

#include <avr/io.h>

enum Prescaler {
        AUCUN = 1,
        PRESCALER_8 = 8,
        PRESCALER_64 = 64,
        PRESCALER_256 = 256,
        PRESCALER_1024 = 1024
    };

class Minuterie {
public:
    Minuterie();
    ~Minuterie() {}

    void demarrer(uint16_t periodeMs, Prescaler prescaler = PRESCALER_1024);
    void arreter();
    void reinitialiser();

};

#endif
