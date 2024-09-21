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

Machine à états : 
+--------------+-----------------+--------------+--------------+
| État présent | Entrée (Bouton) | État suivant | Sortie (Del) |
+--------------+-----------------+--------------+--------------+
| APPUI1       | 1               | APPUI2       | éteinte      |
| APPUI2       | 1               | APPUI3       | éteinte      |
| APPUI3       | 1               | DEL_VERTE    | éteinte      |
| DEL_VERTE    | 0               | APPUI1       | verte        |
+--------------+-----------------+--------------+--------------+

*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

enum class Etat {
    APPUI1,
    APPUI2,
    APPUI3,
    DEL_VERTE
};

constexpr uint8_t DELAI_BOUTON = 30;
constexpr uint16_t TEMPS_AFFICHAGE = 2000; 

void detecterPressionRelachementBouton(Etat& etatActuel, Etat etatSuivant) {
    if (PIND & (1 << PD2)) {
        _delay_ms(DELAI_BOUTON); 
        if (!(PIND & (1 << PD2))) {
            etatActuel = etatSuivant;
        } 
    } 
}

void allumerDelVerte() {
    PORTA |= (1 << PA1); 
    PORTA &= ~(1 << PA0); 
}

void changerEtat(Etat& etatActuel) {
    switch (etatActuel) {
        case Etat::APPUI1 :
            PORTA &= ~(1 << PA1); 
            detecterPressionRelachementBouton(etatActuel, Etat::APPUI2);
            break;

        case Etat::APPUI2 :
            detecterPressionRelachementBouton(etatActuel, Etat::APPUI3);
            break;

        case Etat::APPUI3 :
            detecterPressionRelachementBouton(etatActuel, Etat::DEL_VERTE);
            break;

        case Etat::DEL_VERTE :
            allumerDelVerte();
            _delay_ms(TEMPS_AFFICHAGE);
            etatActuel = Etat::APPUI1;
            break;
    }
}

int main() {
    DDRA |= (1 << PA0) | (1 << PA1); 
    DDRD &= ~(1 << PD2); 

    Etat etatCourant = Etat::APPUI1;

    while(true) {
        changerEtat(etatCourant); 
    }

    return 0;
}
