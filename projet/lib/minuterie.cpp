/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Implémentation des méthodes de la classe `Minuterie`.
Le programme configure une minuterie du microcontrôleur AVR en utilisant différents prescalers pour démarrer la minuterie
avec une durée précise, ainsi que des méthodes pour l'arrêter ou la réinitialiser.

Identifications des matérielles :
- Input :
        - Aucune
- Output :
        - Minuterie interne du microcontrôleur

*/

#include "minuterie.h"

Minuterie::Minuterie() {
    cli();
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
    sei();
}

constexpr uint16_t MS_PAR_SECONDE = 1000;
constexpr uint16_t PERIODE_MINIMALE_MS = 1;

void Minuterie::demarrer(uint16_t periodeMs, Prescaler prescaler) {
    if (periodeMs == 0) {
        periodeMs = 1;
    }

    uint32_t frequenceDesInterruptions = MS_PAR_SECONDE / periodeMs;
    OCR1A = (F_CPU / (prescaler * frequenceDesInterruptions)) - PERIODE_MINIMALE_MS;

    TCNT1 = 0;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    if (prescaler == AUCUN) {
        TCCR1B |= (1 << CS10);
    } else if (prescaler == PRESCALER_8) {
        TCCR1B |= (1 << CS11);
    } else if (prescaler == PRESCALER_64) {
        TCCR1B |= (1 << CS11) | (1 << CS10);
    } else if (prescaler == PRESCALER_256) {
        TCCR1B |= (1 << CS12);
    } else if (prescaler == PRESCALER_1024) {
        TCCR1B |= (1 << CS12) | (1 << CS10);
    }
}

void Minuterie::arreter() {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TIMSK1 &= ~(1 << OCIE1A);
    TCNT1 = 0;
}

void Minuterie::reinitialiser() {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TCNT1 = 0;
}

bool Minuterie::estExpiree() {
    if (TIFR1 & (1 << OCF1A)) {
        TIFR1 |= (1 << OCF1A);
        return true;
    }
    return false;
}
