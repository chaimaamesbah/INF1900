/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Ce programme gère l'interaction avec un bouton connecté à une broche du microcontrôleur AVR. 
Il détecte les pressions et les relâchements avec une gestion anti-rebond. En fonction de la broche utilisée (PD2 ou PD3), 
il active ou désactive les interruptions.

Identifications des matérielles : 
- Input :
        - PD2 ou PD3 (bouton)
- Output :
        - Aucune
*/

#ifndef BOUTON_H
#define BOUTON_H

#include <avr/io.h>
#include <util/delay.h>

class Bouton {
public:
    Bouton(uint8_t pinBouton);
    ~Bouton() {}

    bool estAppuyer();
    bool estRelacher();
    void desactiverInterruption();

private:
    uint8_t pinBouton_;
};

#endif
