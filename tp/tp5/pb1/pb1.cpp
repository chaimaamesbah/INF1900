#define F_CPU 8000000UL
#include <avr/io.h>
#include <memoire_24.h>
#include <string.h>
#include <util/delay.h>
Memoire24CXXX memoire;

void allumerDelRouge() {
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void allumerDelVerte() {
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void ecrireMessage(char *messageEcriture) {
    uint16_t adresse = 0x0000;
    for (uint8_t i = 0; messageEcriture[i] != '\0'; i++) {
        memoire.ecriture(adresse++, messageEcriture[i]);
        _delay_ms(5);
    }
    memoire.ecriture(adresse, 0x00);
}

void lireMessage(char *messageLecture) {
    uint16_t adresse = 0x0000;
    for (uint8_t i = 0; i < 50; i++) {
        memoire.lecture(adresse++, reinterpret_cast<uint8_t *>(&messageLecture[i]));
        if (messageLecture[i] == 0x00) {
            break;
        }
    }
}

int main() {
    DDRA |= (1 << PA0) | (1 << PA1);

    char messageEcriture[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    char messageLecture[50];

    ecrireMessage(messageEcriture);
    lireMessage(messageLecture);

    if (strcmp(messageEcriture, messageLecture) == 0) {
        allumerDelVerte();
    } else {
        allumerDelRouge();
    }
}
