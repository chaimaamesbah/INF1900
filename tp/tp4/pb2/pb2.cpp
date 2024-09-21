#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t gMinuterieExpiree;
volatile uint8_t gBoutonPoussoir;
constexpr uint16_t DUREE_MINUTERIE = 7812;
constexpr uint16_t DELAI_DE_DEPART = 10000;
constexpr uint8_t DELAI_ANTI_REBOND = 30;
constexpr uint8_t DELAI_DEL_ROUGE = 100;

constexpr uint8_t VALEUR_ZERO = 0;
constexpr uint8_t VALEUR_UN = 1;

void initialisation() {
    cli();

    DDRA |= (1 << PA0) | (1 << PA1); 
    DDRD &= ~(1 << PD2); 

    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    EIMSK |= (1 << INT0);

    sei();
}

void allumerDelRouge() {
    PORTA |= (1 << PA0);  
    PORTA &= ~(1 << PA1); 
}

void allumerDelVerte() {
    PORTA |= (1 << PA1);  
    PORTA &= ~(1 << PA0); 
}

void eteindreDel() {
    PORTA &= ~(1 << PA0); 
    PORTA &= ~(1 << PA1); 
}

// Vérifier si nécessaire
void clignoterDelRouge(){
    for (int i = 0; i < 4; i++) {
        allumerDelRouge;
        _delay_ms(10);
        eteindreDel();
        _delay_ms(10);
    }
}

void partirMinuterie() {
    gMinuterieExpiree = 0;
    TCNT1 = 0;
    OCR1A = DUREE_MINUTERIE;
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}

ISR (TIMER1_COMPA_vect) {
    gMinuterieExpiree = 1;
}

ISR (INT0_vect) {
    gBoutonPoussoir = 1;
    _delay_ms(DELAI_ANTI_REBOND);
}

int main() {
    initialisation();

    _delay_ms(DELAI_DE_DEPART);
    
    allumerDelRouge();
    _delay_ms(DELAI_DEL_ROUGE);
    eteindreDel();

    partirMinuterie();

    do {

    } while (gMinuterieExpiree == 0 && gBoutonPoussoir == 0);

    cli ();

    if (gBoutonPoussoir) {
        allumerDelVerte();
    } else if (gMinuterieExpiree) {
        allumerDelRouge();
    }

    return 0;
}
