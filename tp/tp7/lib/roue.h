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

#ifndef ROUE_H
#define ROUE_H

#include <avr/io.h>

enum Roues {
    GAUCHE,
    DROITE
};

enum Direction {
    AVANCER,
    RECULER
};

class Roue {
public:
    Roue();
    ~Roue() {}
    
    // Avancer / Reculer une seule roue 
    void controlerRoue(Roues roue, Direction direction, uint8_t vitessePourcentage);

    // Avancer / Reculer les deux roues ensemble
    void avancer(uint8_t vitessePourcentage); 
    void reculer(uint8_t vitessePourcentage);

    // Tourner :
    void tournerGauche(uint8_t vitessePourcentage, uint8_t reductionVitesse = 2);
    void tournerDroite(uint8_t vitessePourcentage, uint8_t reductionVitesse = 2);
    void tournerSurPlace(uint8_t vitessePourcentage);

    // Accélérer / Ralentir une seule roue
    void accelerer(Roues roue, uint8_t vitesseCible, uint8_t increment);
    void ralentir(Roues roue, uint8_t vitesseCible, uint8_t increment);

    // Arrêter les roues
    void arreter();

    // Obetenir vitesse des roues
    uint8_t obtenirVitesseGauche();
    uint8_t obtenirVitesseDroite();

private:
    static constexpr uint8_t brocheRoueGauche_ = PD6;
    static constexpr uint8_t brocheRoueDroite_ = PD7;

    static constexpr uint8_t directionRoueGauche_ = PD4;
    static constexpr uint8_t directionRoueDroite_ = PD5;

    uint8_t vitesseGauche_;
    uint8_t vitesseDroite_;

    void definirVitesse(Roues roue, Direction direction, uint8_t vitessePourcentage);
};

#endif
