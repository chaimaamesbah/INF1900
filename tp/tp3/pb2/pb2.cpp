/*
Noms des auteurs : ABDEJALIL, Imane (2240526) et MESBAH, Chaïmaa (2342801)
Travail : TP2 - Machines à état
Section # : SECTION 01
Équipe # : EQUIPE_03
Correcteur : Faneva

Description du programme : Au départ, la DEL est éteinte. L'utilisateur doit appuyer et relâcher le bouton trois fois. 
Après la troisième pression, la DEL s'allume en vert pendant 2 secondes, puis s'éteint et retourne à l'état initial, prête à recommencer le cycle.

Identifications des matérielles : 
- Input :
        - D2 (bouton)
- Output :
        - A0 (broche +  de la del)
        - A1 (broche -  de la del)
*/
#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <stdbool.h> 
#include <util/delay_basic.h>

constexpr double DELAI_VARIABLE = 0.01;
int FREQUENCE_60 = 60;
int FREQUENCE_400 = 400;
int DELAY_MS = 2000;

void delaiVariableMs(double delai)
{
    double delaiInterne = delai;
    while (delaiInterne > DELAI_VARIABLE)
    {
        _delay_ms(DELAI_VARIABLE);
        delaiInterne -= DELAI_VARIABLE;
    }
    if (0 < delaiInterne && delaiInterne < DELAI_VARIABLE)
    {
        _delay_ms(DELAI_VARIABLE);//Ca peut erroner les calcul j ai majore le temps restant a 0.1
    }
}

void tournerMoteur(int frequence ,int PWM)
{
    double periode = 1/frequence;
    double delaiMoteurActive = ( periode * PWM ) / 100;
    double delaiMoteurDesactive = periode - delaiMoteurActive;

    PORTB |= (1<<PB0);//Enable=1
    PORTB |= (1<<PB1);//D=1
    delaiVariableMs(delaiMoteurActive);

    PORTB &= ~(1<<PB1);//D=0
    delaiVariableMs(delaiMoteurDesactive);

}

int main ()
{
    DDRB = (1<<PB0)|(1<<PB1);
    DDRD &= ~(1<<PD2);
    
    for (int i = 0 ; i <= 100 ;i += 25)
    {
        tournerMoteur (FREQUENCE_60 , i);
    }
        for (int i = 0 ; i <= 100 ;i += 25)
    {
        tournerMoteur (FREQUENCE_400 , i);
    }
}
