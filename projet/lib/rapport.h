/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier contient l'implémentation des méthodes de la classe `Rapport`. 
Cette classe est utilisée pour générer un rapport détaillé sur la progression d'un robot. Elle permet de définir les points de départ, l'orientation, l'extrémité trouvée, et les positions des poteaux durant la traversée du parcours.
Les rapports sont envoyés via USART pour un suivi en temps réel.

Méthodes :
- `void definirpointDepartExtremite(Point point)` : Définit le point de départ de la recherche d'extrémité (POINT_A ou POINT_B).
- `void definirpointDepartParcours(Point point)` : Définit le point de départ du parcours (POINT_C ou POINT_D).
- `void definirOrientation(Orientation orientation)` : Définit l'orientation du robot (NORD_EST, NORD_OUEST, SUD_EST, SUD_OUEST).
- `void definirExtremiteTrouvee(Point extremite)` : Définit l'extrémité trouvée par le robot.
- `void definirPoteau1(int position)` : Définit la position du poteau 1 (entre 1 et 2).
- `void definirPoteau2(int position)` : Définit la position du poteau 2 (entre 3 et 4).
- `void definirPoteau3(int position)` : Définit la position du poteau 3 (entre 5 et 6).
- `const char* pointEnString(Point point)` : Convertit un point en chaîne de caractères (A, B, C, D, E).
- `const char* orientationEnString(Orientation nouvelleOrientation)` : Convertit une orientation en chaîne de caractères (NE, NO, SE, SO).
- `void envoyerRapport()` : Envoie le rapport complet via USART, incluant les informations sur les points, l'orientation et les poteaux.

Identifications matérielles : 
- Input :
    - Définition des points et orientations par l'utilisateur.
- Output :
    - Rapport envoyé via communication USART.

Données privées :
- `pointDepartExtremite_` : Le point de départ pour la recherche de l'extrémité.
- `pointDepartParcours_` : Le point de départ du parcours.
- `orientation_` : L'orientation initiale du robot.
- `extremiteTrouvee_` : L'extrémité trouvée par le robot.
- `poteau1_`, `poteau2_`, `poteau3_` : Les positions des poteaux rencontrés durant le parcours.
- `numeroEquipe_` : Le numéro d'équipe.
- `nomRobot_` : Le nom du robot.


Machine a états:

| **État**                 | **Condition**                               | **Suivant**         | **Action**                                         |
|--------------------------|---------------------------------------------|---------------------|------------------------------------------          |
| **SUIVRE_LIGNE**         | - Le robot est en mode suivi de ligne.      | SUIVRE_LIGNE        | Suivre la ligne en ajustant les moteurs.           |
|                          | - 3+ capteurs allumés.                      | VERIFIER_CAPTEURS   | Vérifier les capteurs pour ajuster la trajectoire. |
| **VERIFIER_CAPTEURS**    | - 5 capteurs allumés, robot est sur  ligne. | SUIVRE_LIGNE        | Continuer à suivre la ligne.                       |
|                          | - 0 capteur allumé.                         | SUIVRE_LIGNE        | Réajuster la trajectoire et rechercher la ligne.   |
|                          | - 3 capteurs allumés et ligne mal détectée. | CAPTEUR_GAUCHE      | Vérifier le capteur gauche pour ajuster.           |
| **CAPTEUR_GAUCHE**       | - Le capteur gauche détecte la ligne.       | SUIVRE_LIGNE        | Ajuster la trajectoire à gauche.                   |
|                          | - Aucun capteur gauche actif.               | CAPTEUR_DROIT       | Vérifier le capteur droit pour ajuster.            |
| **CAPTEUR_DROIT**        | - Le capteur droit détecte la ligne.        | SUIVRE_LIGNE        | Ajuster la trajectoire à droite.                   |
|                          | - Aucun capteur droit actif.                | VERIFIER_GAUCHE     | Vérifier le capteur gauche pour ajuster.           |
| **VERIFIER_GAUCHE**      | - Ligne détectée à gauche.                  | SUIVRE_LIGNE        | Incrémenter la position gauche.                    |
|                          | - Aucun capteur gauche allumé.              | VERIFIER_DROITE     | Passer à la vérification du capteur droit.         |
| **VERIFIER_DROITE**      | - Ligne détectée à droite.                  | SUIVRE_LIGNE        | Incrémenter la position droite.                    |
|                          | - Aucun capteur droit allumé.               | EXTREMITE_TROUVEE   | Confirmer l'atteinte de l'extrémité.               |
| **EXTREMITE_TROUVEE**    | - Aucun capteur activé.                     | FIN                 | Arrêter les moteurs et signaler l'extrémité trouvée|
| **FIN**                  | - Processus terminé.                        | AUCUN_MODE          | Arrêter le robot et réinitialiser l'état.          |


*/

#ifndef RAPPORT_H
#define RAPPORT_H

#include <avr/io.h>
#include "usart.h"

enum Point {
    POINT_A = 'A',
    POINT_B = 'B',
    POINT_C = 'C',
    POINT_D = 'D',
    POINT_E = 'E',
    AUCUN_POINT
};

enum Orientation {
    NORD_EST,
    NORD_OUEST,
    SUD_EST,
    SUD_OUEST,
    AUCUNE_ORIENTATION
};

class Rapport {
public:
    Rapport() {}

    void definirpointDepartExtremite(Point point);
    void definirpointDepartParcours(Point point);
    void definirOrientation(Orientation orientation);
    void definirExtremiteTrouvee(Point extremite);
    
    void definirPoteau1(int position);
    void definirPoteau2(int position);
    void definirPoteau3(int position);

    const char* orientationEnString(Orientation nouvelleOrientation);
    const char* pointEnString(Point point);

    void envoyerRapport();

private:
    Point pointDepartExtremite_;
    Point pointDepartParcours_;
    Orientation orientation_;
    Point extremiteTrouvee_;
    int poteau1_;
    int poteau2_;
    int poteau3_;

    const char* numeroEquipe_ = "0314";
    const char* nomRobot_ = "Moulay Sultan";
};

#endif
