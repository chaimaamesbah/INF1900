/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Ce fichier contient la déclaration de la classe Del, qui permet de contrôler une DEL (rouge, verte ou ambre) connectée au microcontrôleur. 
Les méthodes permettent d'allumer, éteindre et faire clignoter la DEL en fonction de la couleur choisie.

Identifications des matérielles : 
- Input :
        - Aucune
- Output :
        - PB0 (broche +  de la del)
        - PB1 (broche -  de la del)
*/

#ifndef DEL_H
#define DEL_H

#include <avr/io.h>
#include <util/delay.h>

enum Couleur {
    ROUGE,
    VERT,
    AMBRE
};

class Del {
public:
    Del();
    ~Del() {}
    
    void allumer(Couleur couleur, uint16_t delai = 0);
    void eteindre();
    void clignoter(Couleur couleur, uint8_t nombreClignotements, uint16_t delai);

private:
    void allumerRouge();
    void allumerVert();
    void allumerAmbre();
    void delayMsVariable(uint16_t ms);

    const uint8_t pinRouge_ = PB0;
    const uint8_t pinVert_ = PB1;
};

#endif
