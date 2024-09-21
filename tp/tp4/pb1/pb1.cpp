#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

enum class Etat {
    DEL_AMBRE,
    DEL_VERTE,
    DEL_ROUGE,
    DEL_ETEINTE
};

volatile Etat etatCourant = Etat::DEL_ROUGE;  
constexpr uint8_t DELAI_DEL_ROUGE = 2;
constexpr uint8_t DELAI_DEL_VERTE = 10;
constexpr uint8_t DELAI_ANTI_REBOND = 30;

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

void allumerDelAmbre() {
    allumerDelRouge();
    _delay_ms(DELAI_DEL_ROUGE);
    allumerDelVerte();
    _delay_ms(DELAI_DEL_VERTE);
} 

void eteindreDel() {
    PORTA &= ~(1 << PA0); 
    PORTA &= ~(1 << PA1); 
}

void gererEtatDel() {
    switch (etatCourant) {
        case Etat::DEL_AMBRE :
            for (int i = 0; i < 20; i++) {
                allumerDelAmbre(); 
                if (etatCourant != Etat::DEL_AMBRE) {
                    break; 
                }
            }
            break;

        case Etat::DEL_VERTE :
            allumerDelVerte();
            break;

        case Etat::DEL_ROUGE :
            allumerDelRouge();
            break;

        case Etat::DEL_ETEINTE :
            eteindreDel();
            break;
    }
}

ISR (INT0_vect) {
    _delay_ms(DELAI_ANTI_REBOND);

    if (PIND & (1 << PD2)) {
        if (etatCourant == Etat::DEL_ROUGE) {
            etatCourant = Etat::DEL_AMBRE;
        } else if (etatCourant == Etat::DEL_VERTE) {
            etatCourant = Etat::DEL_ROUGE;
        } else if (etatCourant == Etat::DEL_ETEINTE) {
            etatCourant = Etat::DEL_VERTE;
        }
    } else if (!(PIND & (1 << PD2))) {  
        if (etatCourant == Etat::DEL_AMBRE) {
            etatCourant = Etat::DEL_VERTE;
        } else if (etatCourant == Etat::DEL_ROUGE) {
            etatCourant = Etat::DEL_ETEINTE;
        } else if (etatCourant == Etat::DEL_VERTE) {  
            etatCourant = Etat::DEL_ROUGE;
        }
    }

    EIFR |= (1 << INTF0); 
}

int main() {
    initialisation();

    while (true) {
        gererEtatDel(); 
    }

    return 0;
}
