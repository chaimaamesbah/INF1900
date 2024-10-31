/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Lorsque le bouton sur la breadboard est pressé, la DEL s'allume en rouge (le clic n'est pris en 
compte que si le bouton sur la carte mère n'a pas été relâché et avant la fin d'un délai de 5 secondes) .
Si le bouton sur la carte mère est relâché ou que le délai de 5 secondes s'achève, la DEL clignote en vert et 
les roues exécutent une séquence de mouvements : avancer, tourner à gauche, avancer, puis reculer. 
Le tout est géré de manière asynchrone à l'aide d'interruptions.

Identifications des matérielles : 
- Input :
        - PD2 (bouton sur la carte mère)
        - PD3 (bouton sur le breadboard (plaque essai))
- Output :
        - PD6 (enable roue gauche)
        - PD7 (enable roue droite)
        - PD4 (direction roue gauche)
        - PD5 (direction roue droite)
        - PB0 (broche +  de la del)
        - PB1 (broche -  de la del)
*/

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "bouton.h"
#include "can.h"
#include "del.h"
#include "minuterie.h"
#include "roue.h"
#include "debug.h"

Del del;
Bouton boutonCarteMere(PD2);
Bouton boutonPlaqueEssai(PD3);
Minuterie minuterie;
Roue roue;

constexpr uint16_t PERIODE_MINUTERIE_MS = 5000;
constexpr uint8_t NOMBRE_CLIGNOTEMENTS_VERTE = 5;
constexpr uint8_t DELAI_CLIGNOTEMENT_VERTE_MS = 100;
constexpr uint16_t DELAI_ALLUMAGE_CLIGNOTEMENT_VERTE_MS = 500;
constexpr uint16_t DELAI_DEUX_SECONDES = 2000;
constexpr uint16_t VITESSE_MAXIMALE = 100;

volatile uint8_t gCompteurLimiteAtteinte = 0;
volatile uint8_t gBoutonRelacheDetecte = 0;

ISR(INT0_vect) {
    if (boutonCarteMere.estRelacher()) {
        gBoutonRelacheDetecte = 1;
    }
    
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect) {
    if (boutonPlaqueEssai.estAppuyer()) {
        del.allumer(ROUGE);
    }
    
    EIFR |= (1 << INTF1);
}

ISR(TIMER1_COMPA_vect) {
    gCompteurLimiteAtteinte = 1;
}

int main() {
    minuterie.demarrer(PERIODE_MINUTERIE_MS);

    while (1) {
        if (gCompteurLimiteAtteinte || gBoutonRelacheDetecte) {
            cli();

            DEBUG_PRINT_NUM(VITESSE_MAXIMALE);
            DEBUG_PRINT_CHAR("Bonjour");

            del.clignoter(VERT, NOMBRE_CLIGNOTEMENTS_VERTE, DELAI_CLIGNOTEMENT_VERTE_MS);
            _delay_ms(DELAI_ALLUMAGE_CLIGNOTEMENT_VERTE_MS);
            del.eteindre();

            roue.avancer(VITESSE_MAXIMALE);
            _delay_ms(DELAI_DEUX_SECONDES);

            roue.tournerGauche(VITESSE_MAXIMALE);
            _delay_ms(DELAI_DEUX_SECONDES);

            roue.avancer(VITESSE_MAXIMALE);
            _delay_ms(DELAI_DEUX_SECONDES);

            roue.reculer(VITESSE_MAXIMALE);
            _delay_ms(DELAI_DEUX_SECONDES);
            
            gCompteurLimiteAtteinte = 0;
            gBoutonRelacheDetecte = 0;
            minuterie.reinitialiser(); 

            sei();
        }
    }
}
