/*
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1995
 *
 * Matthew Khouzam et Jerome Collin
 * 2005-2006
 *
 * Code qui n'est sous aucune license.
 *
 */

/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Classe permettant l'accès au convertisseur analogique/numérique
du microcontrôleur ATMega16 de Atmel, avec un décalage de deux bits pour ajuster la précision de la lecture.

Identifications des matérielles : 
    - Input :
            - Port A (0-7)
    - Output :
            - Aucune
*/

#ifndef CAN_H
#define CAN_H

#include <avr/io.h>

class can {
public:
    can();
    ~can();

    uint8_t lecture(uint8_t pos);

private:
    // Donnees membres - aucun
};

#endif
