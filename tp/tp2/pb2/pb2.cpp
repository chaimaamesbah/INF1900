/*
Noms des auteurs : ABDEJALIL, Imane (2240526) et MESBAH, Chaïmaa (2342801)
Travail : TP2 - Machines à état
Section # : SECTION 01
Équipe # : EQUIPE_03
Correcteur : Faneva

Description du programme : Au démarrage, la DEL s'allume en rouge. Si on appuie sur le bouton, la DEL devient ambre. 
En relâchant le bouton, elle passe au vert. Si on appuie à nouveau, elle retourne au rouge. Quand on relâche, la DEL s'éteint. 
Si on appuie encore une fois, elle devient verte, et en la relâchant, elle revient au rouge, prête à recommencer le cycle.

Identifications des matérielles : 
- Input :
        - D2 (bouton)
- Output :
        - A0 (broche +  de la del)
        - A1 (broche -  de la del)
    
Machine à états : 
+--------------+-------------+-----------+--------------+
| État présent | D2 relaché  | D2 appuyé | Sortie (DEL) |
+--------------+-------------+-----------+--------------+
| INIT         | INIT        | DEL_AMBRE | rouge        |
| DEL_AMBRE    | DEL_VERTE   | DEL_AMBRE | ambre        |
| DEL_VERTE    | INIT        | DEL_ROUGE | verte        |
| DEL_ROUGE    | DEL_ETEINTE | DEL_ROUGE | rouge        |
| DEL_ETEINTE  | DEL_ETEINTE | DEL_VERTE | éteinte      |
+--------------+-------------+-----------+--------------+

*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

enum class Etat {
    INIT,
    DEL_AMBRE,
    DEL_VERTE,
    DEL_ROUGE,
    DEL_ETEINTE
};

constexpr uint8_t DELAI_DEL_ROUGE = 2;
constexpr uint8_t DELAI_DEL_VERTE = 10;
constexpr uint8_t DELAI_BOUTON = 30;

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

void detecterPressionBouton(Etat& etatCourant, Etat etatSuivant) {
    if (PIND & (1 << PD2)) {
        _delay_ms(DELAI_BOUTON);
        if (PIND & (1 << PD2)) { 
            etatCourant = etatSuivant;
        }
    }
}

void detecterRelachementBouton(Etat& etatCourant, Etat etatSuivant) {
    if (!(PIND & (1 << PD2))) {
        _delay_ms(DELAI_BOUTON);
        if (!(PIND & (1 << PD2))) { 
            etatCourant = etatSuivant;
        }
    }
}

void gererEtatDel(Etat& etatCourant, bool& estChoixDelVerte) {
    switch (etatCourant) {
        case Etat::INIT :
            allumerDelRouge();
            detecterPressionBouton(etatCourant, Etat::DEL_AMBRE);
            break;

        case Etat::DEL_AMBRE :
            estChoixDelVerte = true;
            for (int i = 0; i < 20; i++) {
                allumerDelAmbre(); 
                detecterRelachementBouton(etatCourant, Etat::DEL_VERTE);
                if (etatCourant != Etat::DEL_AMBRE) {
                    break; 
                }
            }
            break;

        case Etat::DEL_VERTE :
            if (estChoixDelVerte) {
                allumerDelVerte();
                detecterPressionBouton(etatCourant, Etat::DEL_ROUGE);
            } else {
                allumerDelVerte();
                detecterRelachementBouton(etatCourant, Etat::INIT);
            }
            break;

        case Etat::DEL_ROUGE :
            allumerDelRouge();
            detecterRelachementBouton(etatCourant, Etat::DEL_ETEINTE);
            break;

        case Etat::DEL_ETEINTE :
            estChoixDelVerte = false;
            eteindreDel();
            detecterPressionBouton(etatCourant, Etat::DEL_VERTE);
            break;
    }
}

int main() {
    DDRA |= (1 << PA0) | (1 << PA1); 
    DDRD &= ~(1 << PD2); 

    Etat etatCourant = Etat::INIT;
    bool estChoixDelVerte = true;

    while (true) {
        gererEtatDel(etatCourant, estChoixDelVerte); 
    }

    return 0;
}
