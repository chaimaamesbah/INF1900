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

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "del.h"

constexpr uint8_t DUREE_ALLUMAGE_VERTE_MS = 10;
constexpr uint8_t DUREE_ALLUMAGE_ROUGE_MS = 2;
constexpr uint8_t DUREE_MINIMALE_DELAY_MS = 10;

void Del::allumerRouge() {
    PORTB |= (1 << pinRouge_); 
    PORTB &= ~(1 << pinVert_); 
}

void Del::allumerVert() {
    PORTB |= (1 << pinVert_);   
    PORTB &= ~(1 << pinRouge_);
}

void Del::allumerAmbre() {
    allumerRouge();
    _delay_ms(DUREE_ALLUMAGE_ROUGE_MS);
    allumerVert();
    _delay_ms(DUREE_ALLUMAGE_VERTE_MS);
}

void Del::delayMsVariable(uint16_t ms) {
    while (ms > 0) {
        if (ms >= DUREE_MINIMALE_DELAY_MS) {
            _delay_ms(DUREE_MINIMALE_DELAY_MS);
            ms -= DUREE_MINIMALE_DELAY_MS;
        } else {
            _delay_ms(1);
            ms -= 1;
        }
    }
}

Del::Del() {
    DDRB |= (1 << pinRouge_) | (1 << pinVert_);
}

void Del::allumer(Couleur couleur, uint16_t delai) {
    if (couleur == ROUGE) {
        allumerRouge();
    } else if (couleur == VERT) {
        allumerVert();
    } else if (couleur == AMBRE) {
        allumerAmbre();
    } else {
        eteindre();
    }
    delayMsVariable(delai);
}

void Del::eteindre() {
    PORTB &= ~(1 << pinRouge_);
    PORTB &= ~(1 << pinVert_);
}

void Del::clignoter(Couleur couleur, uint8_t nombreClignotements, uint16_t delai) {
    for (int i = 0; i < nombreClignotements; i++) {
        allumer(couleur, delai);
        eteindre();
        delayMsVariable(delai);
    }
}
