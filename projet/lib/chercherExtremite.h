/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier contient l'implémentation des méthodes de la classe `ChercherExtremite`. 
Cette classe est utilisée pour diriger un robot à travers une ligne jusqu'à l'identification d'une extrémité. 
Elle repose sur une machine à états permettant d'exécuter différentes étapes, telles que suivre une ligne, vérifier les capteurs et ajuster la trajectoire.

Méthodes :
- `void demarrer()` : Lance le processus de recherche d'extrémité en initialisant l'état et les compteurs.
- `void trouverExtremite()` : Machine à états qui gère la logique de recherche de l'extrémité.
- `void extremiteAtteinte()` : Finalise le processus en mettant à jour les informations et affichant les résultats via DEL et musique.
- `void reinitialiserCompteurs()` : Réinitialise les compteurs liés aux actions effectuées durant la recherche.
- `void definirPositionEtOrientation(Point point, Orientation orientation, Point extremite)` : Définit la position et l'orientation actuelles du robot.
- `void mettreAJourPositionEtOrientation()` : Met à jour la position et l'orientation en fonction des compteurs.

Identifications matérielles : 
- Input :
    - Capteurs de ligne pour détecter la position du robot.
- Output :
    - DEL pour indiquer l'extrémité atteinte.
    - Musique pour signaler la fin du processus.

Machine a états:

    | **État**                 | **Condition**                               | **Suivant**         | **Action**                               |
    |--------------------------|---------------------------------------------|---------------------|------------------------------------------|
    | **SUIVRE_LIGNE**         | - Le robot n'a pas encore tourné a droite.  | SUIVRE_LIGNE        | Tourner droite après le point 6.         |
    |                          | - 3+ capteurs allumés.                      | VERIFIER_CAPTEURS   | Suivre la ligne.                         |
    | **VERIFIER_CAPTEURS**    | - Max = 5, Min = 0.                         | VERIFIER_GAUCHE     | Vérifier capteurs.                       |
    |                          | - Max = 5, Min ≤ 2.                         | SUIVRE_LIGNE        | Incrémenter intersection.                |
    |                          | - Max = 2/3/4, Min ≤ 2, gauche inactif.     | CAPTEUR_GAUCHE      | Ajuster trajectoire.                     |
    |                          | - Max = 2/3/4, Min ≤ 2, droit inactif.      | CAPTEUR_DROIT       | Ajuster trajectoire.                     |
    |                          | - Autres cas.                               | SUIVRE_LIGNE        | Retour au suivi de ligne.                |
    | **VERIFIER_GAUCHE**      | - Ligne trouvée gauche.                     | SUIVRE_LIGNE        | Incrémenter gauche.                      |
    |                          | - Aucun capteur allumé.                     | VERIFIER_DROITE     | Vérifier droite.                         |
    | **VERIFIER_DROITE**      | - Ligne trouvée droite.                     | SUIVRE_LIGNE        | Incrémenter droite.                      |
    |                          | - Aucun capteur allumé.                     | EXTREMITE_TROUVEE   | Confirmer extrémité trouvée.             |
    | **CAPTEUR_GAUCHE**       | Aucun.                                      | SUIVRE_LIGNE        | Ajuster trajectoire gauche.              |
    | **CAPTEUR_DROIT**        | Aucun.                                      | SUIVRE_LIGNE        | Ajuster trajectoire droite.              |
    | **EXTREMITE_TROUVEE**    | Aucun.                                      | Fin                 | Arrêter roues, extrémité trouvée.        |

*/

#ifndef CHERCHEREXTREMITE_H
#define CHERCHEREXTREMITE_H

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "capteurLigne.h"
#include "rapport.h"
#include "del.h"
#include "musique.h"
#include "usart.h"
#include "debug.h"
#include "roue.h"

namespace ChercherExtremiteEtats {
    enum EtatRobot {
        SUIVRE_LIGNE,
        VERIFIER_CAPTEURS_ALLUME,
        VERIFIER_EXTREMITE_GAUCHE,
        VERIFIER_EXTREMITE_DROITE,
        CAPTEUR_GAUCHE_ETEINT,
        CAPTEUR_DROIT_ETEINT,
        EXTREMITE_TROUVER,
    };
}

class ChercherExtremite {
public:
    ChercherExtremite(CapteurLigne* capteurLigne, Roue* roue, Del* del, Musique* musique, Rapport* rapport) 
        : capteurLigne_(capteurLigne),
          roue_(roue),
          del_(del),
          musique_(musique),
          rapport_(rapport),
          pointDepart_(AUCUN_POINT), 
          extremiteTrouver_(AUCUN_POINT), 
          orientation_(AUCUNE_ORIENTATION),
          extremiteTrouvee_(false),
          actionTournerDroiteEffectuee_(false) {}

    void demarrer();
    void trouverExtremite();
    void extremiteAtteinte();

private:
    CapteurLigne* capteurLigne_;
    Roue* roue_;
    Del* del_;
    Musique* musique_;
    Rapport* rapport_;

    uint8_t compteurTournerGauche_ = 0;
    uint8_t compteurTournerDroite_ = 0;
    uint8_t compteurCapteurGaucheEteint_ = 0;
    uint8_t compteurCapteurDroitEteint_ = 0;
    uint8_t compteurIntersection_ = 0;
    

    ChercherExtremiteEtats::EtatRobot etat = ChercherExtremiteEtats::SUIVRE_LIGNE;

    Point pointDepart_;
    Point extremiteTrouver_;
    Orientation orientation_;

    bool extremiteTrouvee_;
    bool actionTournerDroiteEffectuee_;

    void reinitialiserCompteurs();
    void mettreAJourPositionEtOrientation();
    void definirPositionEtOrientation(Point point, Orientation orientation, Point extremite);
};

#endif