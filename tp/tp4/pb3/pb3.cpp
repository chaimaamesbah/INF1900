#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

constexpr uint8_t DELAI_PWM = 255;
constexpr uint16_t DELAI_DES_SIGNAUX = 2000;

void ajusterPWM(float intensitePWM)
{
    // TIMER 1 est sur 16 bits
    OCR1A = 0xFF * intensitePWM;
    OCR1B = 0xFF * intensitePWM;
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << CS11);
    TCCR1C = 0;
}

int main()
{
    DDRD |= 0xFF;

    for (float i = 0; i <= 1; i += 0.25)
    {
        // int intensitePWM = (int)(DELAI_PWM * i);
        ajusterPWM(i);
        _delay_ms(DELAI_DES_SIGNAUX);
    }

    ajusterPWM(0);
}

