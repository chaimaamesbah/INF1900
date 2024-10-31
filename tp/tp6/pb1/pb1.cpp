/*
Noms des auteurs : ABDEJALIL, Imane (2240526) et MESBAH, Chaïmaa (2342801)
Travail : TP6 - Capteurs et conversion analogique/numérique
Section # : SECTION 01
Équipe # : EQUIPE_03
Correcteur : Paul

Description du programme : Le programme contrôle un bouton-poussoir connecté à PD2. 
Quand on appuie, un compteur s'incrémente 10 fois par seconde. Si le bouton est relâché ou si le compteur 
atteint 120, une DEL clignote en vert pendant 0,5 seconde. Deux secondes après, la DEL rouge clignote (compteur/2) 
fois à 2 Hz, puis devient verte pendant 1 seconde avant de s'éteindre. Le système revient ensuite à l'état initial.

Identifications des matérielles : 
- Input :
        - D2 (bouton sur breadboard)
- Output :
        - B0 (broche +  de la del)
        - B1 (broche -  de la del)
*/

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t gCompteurLimiteAtteinte = 0;
volatile uint8_t gBoutonRelacheDetecte = 0;
volatile uint8_t gCompteur = 0;

constexpr uint16_t PERIODE_MINUTERIE_MS = (F_CPU / (1024 * 10)) - 1;
constexpr uint8_t LIMITE_COMPTEUR = 120;
constexpr uint8_t FACTEUR_DIVISION_CLIGNOTEMENT = 2;

constexpr uint8_t DELAI_ANTI_REBOND_MS = 30;
constexpr uint8_t DELAI_CLIGNOTEMENT_VERTE_MS = 100;
constexpr uint16_t DELAI_ALLUMAGE_CLIGNOTEMENT_VERTE_MS = 500;
constexpr uint16_t DELAI_AVANT_CLIGNOTEMENT_ROUGE_MS = 2000;
constexpr uint8_t DELAI_CLIGNOTEMENT_ROUGE_MS = 250;
constexpr uint16_t DUREE_ALLUMAGE_VERTE_MS = 1000;
constexpr uint8_t CONFIGURATION_VIDE_TCCR1A = 0;
constexpr uint8_t NOMBRE_CLIGNOTEMENTS_VERTE = 5;

void initialisation() {
    cli();

    DDRB |= (1 << PB0) | (1 << PB1);
    DDRD &= ~(1 << PD2);

    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);
    EIMSK |= (1 << INT0);

    OCR1A = PERIODE_MINUTERIE_MS;
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

void allumerDelRouge() {
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
}

void allumerDelVerte() {
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB0);
}

void eteindreDel() {
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);
}

void clignoterDelVerte() {
    for (int i = 0; i < NOMBRE_CLIGNOTEMENTS_VERTE; i++) {
        allumerDelVerte();
        _delay_ms(DELAI_CLIGNOTEMENT_VERTE_MS);
        eteindreDel();
        _delay_ms(DELAI_CLIGNOTEMENT_VERTE_MS);
    }
}

void clignoterDelRouge() {
    uint8_t nombreDeClignotements = (gCompteur / FACTEUR_DIVISION_CLIGNOTEMENT);

    for (int i = 0; i < nombreDeClignotements; i++) {
        allumerDelRouge();
        _delay_ms(DELAI_CLIGNOTEMENT_ROUGE_MS);
        eteindreDel();
        _delay_ms(DELAI_CLIGNOTEMENT_ROUGE_MS);
    }
}

ISR(INT0_vect) {
    if ((PIND & (1 << PD2))) {
        _delay_ms(DELAI_ANTI_REBOND_MS);

        if ((PIND & (1 << PD2))) {
            gBoutonRelacheDetecte = 1;
        }
    }

    EIFR |= (1 << INTF0);
}

ISR(TIMER1_COMPA_vect) {
    if (!(PIND & (1 << PD2))) {
        gCompteur++;
    }

    if (gCompteur >= LIMITE_COMPTEUR) {
        gCompteurLimiteAtteinte = 1;
    }
}

int main() {
    initialisation();

    while (1) {
        if (gCompteurLimiteAtteinte || gBoutonRelacheDetecte) {
            cli();

            clignoterDelVerte();
            _delay_ms(DELAI_ALLUMAGE_CLIGNOTEMENT_VERTE_MS);
            eteindreDel();
            _delay_ms(DELAI_AVANT_CLIGNOTEMENT_ROUGE_MS);

            clignoterDelRouge();

            allumerDelVerte();
            _delay_ms(DUREE_ALLUMAGE_VERTE_MS);
            eteindreDel();

            gCompteur = 0;
            gCompteurLimiteAtteinte = 0;
            gBoutonRelacheDetecte = 0;

            sei();
        }
    }

    return 0;
}
