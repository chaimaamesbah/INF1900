#include "musique.h"

Musique::Musique() {
    DDRB |= (1 << PB2) | (1 << PB3);
    PORTB &= ~(1 << PB2);

    TCCR0A &= ~((1 << WGM02) | (1 << WGM00) | (1 << COM0A1));
    TCCR0A |= (1 << WGM01) | (1 << COM0A0);

}

void Musique::jouerNote(double frequence) {
    TCCR0B &= ~((1 << CS01) | (1 << CS00));
    TCCR0B |= (1 << CS02);
    TCNT0 = 0;
    OCR0A = (F_CPU / (PRESCALAGE_MINUTERIE * DIVISEUR_FREQUENCE_MINUTERIE * frequence)) - 1;
}

void Musique::arreter() {
    TCCR0B = 0;
}