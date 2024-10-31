/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Il permet de contrôler individuellement les roues (gauche ou droite), de les faire avancer, 
reculer, et de gérer des actions comme tourner ou accélérer. Pour contrôler la vitesse des roues, nous avons utilisé le Timer 2 en Mode Phase Correct 
avec un prescaler de 64

Identifications des matérielles : 
- Input :
        - Aucune
- Output :
        - PD6 (enable roue gauche)
        - PD7 (enable roue droite)
        - PD4 (direction roue gauche)
        - PD5 (direction roue droite)
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "roue.h"

constexpr uint8_t MAX_VITESSE_POURCENTAGE_ROUE = 100;
constexpr uint8_t RAPPORT_CYCLIQUE_MAX = 255;
constexpr uint8_t DELAI_ACCELERER_MS = 10;
constexpr uint8_t DELAI_RALENTIR_MS = 10;


Roue::Roue() {
    DDRD |= (1 << brocheRoueDroite_) | (1 << brocheRoueGauche_);
    DDRD |= (1 << directionRoueDroite_) | (1 << directionRoueGauche_);

    TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20); 
    TCCR2B |= (1 << CS22);                                 

    OCR2A = 0;
    OCR2B = 0;
    vitesseGauche_ = 0;
    vitesseDroite_ = 0;
}

void Roue::definirVitesse(Roues roue, Direction direction, uint8_t vitessePourcentage) {
    if (vitessePourcentage > MAX_VITESSE_POURCENTAGE_ROUE) {
        vitessePourcentage = MAX_VITESSE_POURCENTAGE_ROUE;
    }

    uint8_t rapportCyclique = (vitessePourcentage * RAPPORT_CYCLIQUE_MAX) / MAX_VITESSE_POURCENTAGE_ROUE;

    if (roue == DROITE) {
        OCR2A = rapportCyclique;
        vitesseDroite_ = vitessePourcentage;
    } else if (roue == GAUCHE) {
        OCR2B = rapportCyclique;
        vitesseGauche_ = vitessePourcentage;
    }

    uint8_t directionPin = (roue == GAUCHE) ? directionRoueGauche_ : directionRoueDroite_;
    if (direction == AVANCER) {
        PORTD &= ~(1 << directionPin);
    } else if (direction == RECULER) {
        PORTD |= (1 << directionPin);
    }
}

// Avancer / Reculer une seule roue
void Roue::controlerRoue(Roues roue, Direction direction, uint8_t vitessePourcentage) {
    definirVitesse(roue, direction, vitessePourcentage);
}

// Avancer / Reculer les deux roues ensemble
void Roue::avancer(uint8_t vitessePourcentage) {
    definirVitesse(GAUCHE, AVANCER, vitessePourcentage);
    definirVitesse(DROITE, AVANCER, vitessePourcentage);
}

void Roue::reculer(uint8_t vitessePourcentage) {
    definirVitesse(GAUCHE, RECULER, vitessePourcentage);
    definirVitesse(DROITE, RECULER, vitessePourcentage);
}

// Tourner
void Roue::tournerGauche(uint8_t vitessePourcentage, uint8_t reductionVitesse) {
    definirVitesse(GAUCHE, AVANCER, vitessePourcentage / reductionVitesse);
    definirVitesse(DROITE, AVANCER, vitessePourcentage);
}

void Roue::tournerDroite(uint8_t vitessePourcentage, uint8_t reductionVitesse) {
    definirVitesse(GAUCHE, AVANCER, vitessePourcentage);
    definirVitesse(DROITE, AVANCER, vitessePourcentage / reductionVitesse);
}

void Roue::tournerSurPlace(uint8_t vitessePourcentage) {
    definirVitesse(GAUCHE, AVANCER, vitessePourcentage);
    definirVitesse(DROITE, RECULER, vitessePourcentage);
}

// Accélérer / Ralentir une roue
void Roue::accelerer(Roues roue, uint8_t vitesseCible, uint8_t increment) {
    uint8_t vitesseActuelle = (roue == GAUCHE) ? vitesseGauche_ : vitesseDroite_;

    while (vitesseActuelle < vitesseCible) {
        definirVitesse(roue, AVANCER, vitesseActuelle);
        _delay_ms(DELAI_ACCELERER_MS);
        vitesseActuelle += increment;
    }
    definirVitesse(roue, AVANCER, vitesseCible);
}

void Roue::ralentir(Roues roue, uint8_t vitesseCible, uint8_t decrement) {
    uint8_t vitesseActuelle = (roue == GAUCHE) ? vitesseGauche_ : vitesseDroite_;

    while (vitesseActuelle > vitesseCible) {
        definirVitesse(roue, RECULER, vitesseActuelle);
        _delay_ms(DELAI_RALENTIR_MS);
        if (vitesseActuelle > decrement) {
            vitesseActuelle -= decrement;
        } else {
            vitesseActuelle = vitesseCible;
        }
    }
    definirVitesse(roue, RECULER, vitesseCible);
}

// Arrêter les roues
void Roue::arreter() {
    OCR2A = 0;
    OCR2B = 0;
    vitesseGauche_ = 0;
    vitesseDroite_ = 0;
}

// Obtenir la vitesse
uint8_t Roue::obtenirVitesseGauche() {
    return vitesseGauche_;
}

uint8_t Roue::obtenirVitesseDroite() {
    return vitesseDroite_;
}
