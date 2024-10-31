/*
Noms des auteurs : ABDEJALIL, Imane (2240526) et MESBAH, Chaïmaa (2342801)
Travail : TP6 - Capteurs et conversion analogique/numérique
Section # : SECTION 01
Équipe # : EQUIPE_03
Correcteur : Paul

Description du programme : Le programme contrôle une DEL en fonction de la lumière ambiante.
Si la lumière est faible (photorésistance cachée), la DEL s'allume en vert. Si la lumière est modérée,
la DEL devient ambrée. Si la lumière est trop forte (sous une lampe), la DEL devient rouge.

Identifications des matérielles :
- Input :
        - A0 (photorésistance)
- Output :
        - B0 (broche +  de la del)
        - B1 (broche -  de la del)
*/

#define F_CPU 8000000UL
#include "can.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

constexpr uint8_t DUREE_ALLUMAGE_VERTE_MS = 10;
constexpr uint8_t DUREE_ALLUMAGE_ROUGE_MS = 2;
constexpr uint8_t SEUIL_MIN = 225;
constexpr uint8_t SEUIL_MAX = 240;
constexpr uint8_t DECALAGE_ADC_VERS_8BITS = 2;

void allumerDelRouge() {
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
}

void allumerDelVerte() {
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB0);
}

void allumerDelAmbre() {
    allumerDelRouge();
    _delay_ms(DUREE_ALLUMAGE_ROUGE_MS);
    allumerDelVerte();
    _delay_ms(DUREE_ALLUMAGE_VERTE_MS);
}

int main() {
    DDRB |= (1 << PB0) | (1 << PB1);
    can canConvertisseur;

    while (true) {
        uint8_t niveauLuminositeAdc = (canConvertisseur.lecture(PA0) >> DECALAGE_ADC_VERS_8BITS);

        if (niveauLuminositeAdc < SEUIL_MIN) {
            allumerDelVerte();
        } else if (niveauLuminositeAdc < SEUIL_MAX) {
            allumerDelAmbre();
        } else {
            allumerDelRouge();
        }
    }
    return 0;
}