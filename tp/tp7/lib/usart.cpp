/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Il contient les definitions des fonctions declarees dans Usart.h 

Identifications des matérielles : 
- Input :
        - PD0 (RX)
- Output :
        - PD1 (TX)

*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "usart.h"

constexpr uint16_t REGISTRE_VITESSE_BAUD = 0xCF;

Usart::Usart() {
    UBRR0H = 0;
    UBRR0L = REGISTRE_VITESSE_BAUD;
    UCSR0A = 0;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void Usart::transmettre(uint8_t donnee) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = donnee;
}

void Usart::transmettreDonne(const char *message) {
    for (uint8_t i = 0; message[i] != '\0'; i++) {
        transmettre(message[i]);
    }
}

void Usart::transmettreDonne(uint8_t donnee) {
    char buffer[12]; 
    sprintf(buffer, "%d", donnee); 
    transmettreDonne(buffer); 
}
int Usart::recevoir() {
    while (!(UCSR0A & (1 << RXC0))) {}
    return UDR0;
}

