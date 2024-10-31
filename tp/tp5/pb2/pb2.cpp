#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

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

// Du USART vers le PC
void transmissionUART(uint8_t donnee) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

void envoyerMessage(const char *message) {
    for (uint8_t i = 0; message[i] != '\0'; i++) {
        transmissionUART(message[i]);
    }
}

int main() {
    char message[] = "Le robot en INF1900\n";

    initialisationUART();
    envoyerMessage(message);
    while(1);
}