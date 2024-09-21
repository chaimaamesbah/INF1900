#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

const int DELAY_LED_ROUGE = 2;
const int DELAY_LED_VERT = 10;
const int DELAY_BOUTON = 30;

void allumerLedRouge() {
    PORTA |= (1 << PA0); // Allume PA0
    PORTA &= ~(1 << PA1); // Éteint PA1
    _delay_ms(DELAY_LED_ROUGE);
}

void allumerLedVerte() {
    PORTA |= (1 << PA1); // Allume PA1
    PORTA &= ~(1 << PA0); // Éteint PA0
    _delay_ms(DELAY_LED_VERT);
}

void controlerLedsAvecBouton() {
    if (PIND & (1 << PD2)) {
            _delay_ms(DELAY_BOUTON);
        // Boucle infinie (del amber)
            if (PIND & (1 << PD2)) { 
                while(PIND & (1 << PD2)) {
                    allumerLedVerte();
                    allumerLedRouge();
                }
            } 
        }
    PORTA &= ~(1 << PA0); // Éteint PA0
    PORTA &= ~(1 << PA1); // Éteint PA1
    }

int main() {
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2);

    while(1) {
        controlerLedsAvecBouton();
    }
    return 0;

}
