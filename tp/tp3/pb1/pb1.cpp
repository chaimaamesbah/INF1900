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

enum class Etat
{
    INIT,
    DEL_VERTE,
    DEL_ROUGE
};

constexpr uint8_t DELAI_ANTI_REBOND = 5;
Etat ETAT_ACTUEL = Etat::INIT;
constexpr double DELAI_VARIABLE = 0.1;
double NOMBRE = 0.001;

bool estBoutonAppuyeAntiRebondEtRelacher()
{
    if (PIND & (1<<PD2))
    {
        _delay_ms(DELAI_ANTI_REBOND);
        if (PIND & (1<<PD2))
        {
            while (PIND & (1<<PD2))
            {
                _delay_ms(DELAI_ANTI_REBOND);
            }
            return true;
        }
    }
    return false;
}

void delaiVariableMs(double delai)
{
    double delaiInterne = delai;
    while (delaiInterne >= DELAI_VARIABLE)
    {
        _delay_ms(DELAI_VARIABLE);
        delaiInterne -= DELAI_VARIABLE;
    }
}

void allumerRougeDureeA(double delai)
{
    double delaiEteinte = 1.0 - delai;
    PORTA &= ~(1<<PA1);
    PORTA |= (1<<PA0);
    delaiVariableMs(delai);
    PORTA &= ~(1<<PA0);
    delaiVariableMs(delaiEteinte);
}

void allumerVertDureeA(double delai)
{
    double delaiEteinte = 1.0 - delai;
    PORTA &= ~(1<<PA0);
    PORTA |= (1<<PA1);
    delaiVariableMs(delai);
    PORTA &= ~(1<<PA1);
    delaiVariableMs(delaiEteinte);
}

int main ()
{
    DDRA = (1<<PA0)|(1<<PA1);
    DDRD &= ~(1<<PD2);
    
    while (true)
    {
        switch (ETAT_ACTUEL)
        {
        case Etat::INIT :
            if (estBoutonAppuyeAntiRebondEtRelacher())
            {
                ETAT_ACTUEL = Etat::DEL_VERTE ;
            }
            break;
        
        case Etat::DEL_VERTE :
            for (int k = 900; k > 10; k--)
            {
                 double delai = NOMBRE * k ;
                allumerVertDureeA(delai);
            }
            ETAT_ACTUEL = Etat::DEL_ROUGE;
            break;
        
        case Etat::DEL_ROUGE :
            for (int k = 900; k > 10; k--)
            {
                double delai = NOMBRE *k ;
                allumerRougeDureeA(delai);
            }
            ETAT_ACTUEL = Etat::INIT;
            break;
        }
    }
    return 0;

}
