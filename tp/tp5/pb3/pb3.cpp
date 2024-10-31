#define F_CPU 8000000UL
#include <memoire_24.h>
#include <string.h>  
#include <util/delay.h>

Memoire24CXXX memoire;

void initialisationUART() {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void trouverValeur() {
    uint16_t adresse = 0x0000;
    uint16_t adresseRechercher = 0xFF;
    uint8_t donneLu;

    do {
        memoire.lecture(adresse++, &donneLu);
    } while (!(donneLu == adresseRechercher));
}

int main() {
    initialisationUART();
    trouverValeur();
    while(1);
}