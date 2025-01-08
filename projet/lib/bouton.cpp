/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Ce fichier contient l'implémentation des méthodes de la classe Bouton. 
Il permet de gérer un bouton physique connecté à une broche du microcontrôleur AVR avec une gestion d'interruptions et un délai anti-rebond. 
Les interruptions sont configurées pour détecter les pressions et relâchements du bouton.

- Bouton(uint8_t pinBouton) : Constructeur qui initialise la broche en entrée et configure les interruptions (INT0 ou INT1) selon la broche utilisée (PD2 ou PD3).
- bool estAppuyer() : Vérifie si le bouton est pressé en appliquant un délai anti-rebond.
- bool estRelacher() : Vérifie si le bouton est relâché, également avec un délai anti-rebond.
- void desactiverInteruption() : Désactive l'interruption associée à la broche du bouton.

Identifications des matérielles : 
- Input :
        - PD2 ou PD3 (bouton physique connecté à la broche)
- Output :
        - Aucune (ce fichier ne gère aucune sortie visuelle)

*/

#include "bouton.h"

const uint8_t DELAI_ANTI_REBOND_MS = 30;

Bouton::Bouton(volatile uint8_t pinBouton) : pinBouton_(pinBouton) {
    cli();
    DDRD &= ~(1 << pinBouton_);

    if (pinBouton_ == PD2) {
        EICRA |= (1 << ISC00); 
        EICRA &= ~(1 << ISC01);
        EIMSK |= (1 << INT0); 
    } else if (pinBouton_ == PD3) {
        EICRA |= (1 << ISC10); 
        EICRA &= ~(1 << ISC11);
        EIMSK |= (1 << INT1);
    }
    sei();
}

bool Bouton::estAppuyer() {
    _delay_ms(DELAI_ANTI_REBOND_MS);
    if (pinBouton_ == PD2)
        return (PIND & (1 << pinBouton_));
    else if (pinBouton_ == PD3)
        return (!(PIND & (1 << pinBouton_)));
    return false;
}

bool Bouton::estRelacher(uint8_t pinBouton) {
    _delay_ms(DELAI_ANTI_REBOND_MS);
    if (pinBouton == PD2)
        return (!(PIND & (1 << pinBouton)));
    else if (pinBouton == PD3)
        return ((PIND & (1 << pinBouton)));
    return false;
}

void Bouton::desactiverInterruption() {
    if (pinBouton_ == PD2)
        EIMSK &= ~(1 << INT0);
    else if (pinBouton_ == PD3)
        EIMSK &= ~(1 << INT1);
}
