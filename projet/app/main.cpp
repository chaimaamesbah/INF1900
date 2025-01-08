/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme :
Ce fichier contient l'implémentation du programme principal qui gère le comportement du robot. 
Il inclut la gestion des différents modes de fonctionnement du robot, tels que la recherche de l'extrémité, la traversée du parcours et l'affichage du rapport. 
Les actions sont contrôlées par une série de capteurs, d'interruptions et de modules, et le programme interagit avec des composants matériels tels que des LEDs, des moteurs, des capteurs de distance et des capteurs de ligne.

Les principaux modules sont :
- TrouverPoteau : Gère la détection des poteaux via les capteurs de distance et de ligne.
- ChercherExtremite : Permet de trouver l'extrémité du parcours à l'aide des capteurs et du moteur.
- Rapport : Envoie un rapport détaillé via USART concernant la progression du robot.
- ModeFonctionnement : Gère les modes du robot et le passage d'un mode à l'autre.

Méthodes principales :
- ISR(INT0_vect) : Gère l'interruption générée par l'appui sur le bouton (Interrupt).
- main() : Gère les différents modes et lance les actions correspondantes selon l'état du robot.

Identifications matérielles :
- Input :
    - Bouton (interrupt)                 : PD2
    - Can                                : Port A (0-7)
    - Del                                : PD4-PD7
    - Capteur distance                   : PA0
    - Capteur ligne a l'extrémité gauche : PC7
    - Capteur ligne gauche-milieu        : PC6
    - Capteur ligne milieu               : PC5
    - Capteur ligne droite-milieu        : PC4
    - Capteur ligne droite               : PC3
    - Usart                              : PD0 (RX)
    - Bouton (RESET) pour changer le mode de fonctionnement.
    
- Output :
    - PD6 (enable roue gauche)
    - PD7 (enable roue droite)
    - PD4 (direction roue gauche)
    - PD5 (direction roue droite)
    - PB3 (PWM)(musique)
    - PB2 (ground)(musique)
    - Rapport envoyé via communication USART.
    

-Machine a états:

| **État actuel**        | **Entree**                                 | **État suivant**         | **Action**                                 |
|------------------------|---------------------------------------     |------------------------- |--------------------------------------------|
| **AUCUN_MODE**         | Bouton reset appuiye pendant del **verte** | `CHERCHER_EXTREMITE`     | appel de fonction de cycle de sélection    |
| **AUCUN_MODE**         | Bouton reset appuiye pendant del **rouge** | `TRAVERSER_PARCOURS`     | idem                                       |
| **AUCUN_MODE**         | Bouton reset appuiye pendant del **ambre** | `AFFICHER_RAPPORT`       | idem                                       |
| **AUCUN_MODE**         | Bouton reset **non appuiye**               | `AUCUN_MODE`             | idem                                       |
|------------------------|---------------------------------------     |------------------------- |--------------------------------------------|
| **CHERCHER_EXTREMITE** | Aucune                                     | `AUCUN_MODE`             | Clignoter **vert**, démarrer recherche     |
| **TRAVERSER_PARCOURS** | Aucune                                     | `AUCUN_MODE`             | Clignoter **rouge**, démarrer parcours     |
| **AFFICHER_RAPPORT**   | Aucune                                     | `AUCUN_MODE`             | Clignoter **ambre**, envoyer rapport       |

*/

#define F_CPU 8000000UL
#include "del.h"
#include "memoire_24.h"
#include "roue.h"
#include "modeFonctionnement.h"
#include "rapport.h"
#include "musique.h"
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "trouverPoteau.h"
#include "can.h"
#include "bouton.h"
#include "capteurDistance.h"
#include "capteurLigne.h"
#include "chercherExtremite.h"

ISR(INT0_vect) {
    TrouverPoteau::gererInterruption();
    EIFR |= (1 << INTF0);
}

int main() {
    Del del;
    Memoire24CXXX memoire;
    Roue roue;
    ModeFonctionnement mode(&del, &memoire);
    Rapport rapport;
    can can;
    Bouton bouton(PD2);
    CapteurDistance capteurDistance(&can, &del);
    CapteurLigne capteurLigne(&roue);
    Musique musique;
    ChercherExtremite chercherExtremite(&capteurLigne, &roue, &del, &musique, &rapport);
    TrouverPoteau trouverPoteau(&can, &capteurLigne, &capteurDistance, &roue, &del, &musique, &rapport, &bouton);

    while(true) {
        Mode modeEnCours = mode.verifierReset();

        switch (modeEnCours) {
            case Mode::AUCUN_MODE:
                mode.lancerCycleSelection();
                break;  

            case Mode::CHERCHER_EXTREMITE :
                mode.enregistrerMode(Mode::AUCUN_MODE);
                del.clignoter(VERT, 8, 125);
                
                _delay_ms(2000);
                chercherExtremite.demarrer();

                modeEnCours = Mode::AUCUN_MODE;
                break;
            
            case Mode::TRAVERSER_PARCOURS :
                mode.enregistrerMode(Mode::AUCUN_MODE);
                del.clignoter(ROUGE, 8, 125);

                trouverPoteau.demarrer();

                modeEnCours = Mode::AUCUN_MODE;
                break;

            case Mode::AFFICHER_RAPPORT :
                mode.enregistrerMode(Mode::AUCUN_MODE);
                del.clignoter(AMBRE, 8, 125);

                for (int i = 0; i < 1; i++) {
                    rapport.envoyerRapport();
                    _delay_ms(1000); 
                }

                modeEnCours = Mode::AUCUN_MODE;
                break;
        }
    }

    return 0;
}