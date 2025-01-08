/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), 
                   SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier implémente la classe `TrouverPoteau`, responsable de détecter les poteaux 
et de naviguer sur un parcours en suivant des lignes. Le programme utilise plusieurs 
capteurs pour analyser l'environnement, détecter des poteaux, et envoyer des rapports 
de progression.

Méthodes principales :
- `void demarrer()` : Point d'entrée pour initialiser et démarrer le processus de détection.
- `void trouverPoteau()` : Gère l'ensemble des états et transitions pour détecter les poteaux.
- `void mettreAJourPositionEtPositionPoteau()` : Met à jour les positions des poteaux détectés.
- `uint8_t verifierPresencePoteau()` : Vérifie et calcule la moyenne de fréquence pour déterminer la présence d’un poteau.
- `void trouverLigne()` : Localise une ligne après une détection ou une perte.
- `void jouerDeuxSonsAigus()` : Joue une séquence de sons aigus pour signaler une fin.
- `void jouerHuitSonsGraves()` : Joue une séquence de sons graves pour signaler la détection d'un poteau.
- `static void gererInterruption()` : Gère les interruptions détectées par le bouton.
- `static void reinitialiserBoutonRelacheDetecte()` : Réinitialise l’état du bouton après une détection.


Identifications matérielles : 
- Input :
    - Utilisation de capteurs pour suivre les lignes et détecter les poteaux.
    - Interruption déclenchée par un bouton (Interrupt) pour démarrer une séquence.
- Output :
    - LED et sons pour signaler l'état du robot.
    - Rapport final généré par la classe `Rapport`.

Machine a états:

| **État Actuel**           | **Action**                                            | **Condition de Transition**                 | **État Suivant**             |
|---------------------------|-------------------------------------------------------|---------------------------------------------|------------------------------|
| `ETAT_INITIAL`            | Initialisation des variables et mise en route.        | Aucun (initialisation terminée).            | `SUIVRE_LIGNE`               |
| `SUIVRE_LIGNE`            | Suivre la ligne à l’aide des capteurs.                | Poteau détecté à 10 cm.                     | `POTEAU_DETECTER_DIX_CM`     |
|                           |                                                       | Intersection détectée.                      | `VERIFIER_CAPTEURS_ALLUME`   |
|                           |                                                       | Extrémité détectée.                         | `EXTREMITE_TROUVER`          |
| `VERIFIER_CAPTEURS_ALLUME`| Analyser l'état des capteurs.                         | Poteau potentiel détecté.                   | `VERIFIER_PRESENCE_POTEAU`   |
|                           |                                                       | Ligne détectée normalement.                 | `SUIVRE_LIGNE`               |
|                           |                                                       | Capteur gauche éteint.                      | `CAPTEUR_GAUCHE_ETEINT`      |
|                           |                                                       | Capteur droit éteint.                       | `CAPTEUR_DROIT_ETEINT`       |
| `VERIFIER_PRESENCE_POTEAU`| Vérifier la fréquence pour confirmer un poteau.       | Fréquence valide (poteau confirmé).         | `SUIVRE_LIGNE`               |
|                           |                                                       | Fréquence invalide.                         | `TROUVER_LIGNE`              |
| `TROUVER_LIGNE`           | Recherche de ligne pour se réaligner.                 | Ligne retrouvée.                            | `VERIFIER_PRESENCE_POTEAU`   |
| `POTEAU_DETECTER_DIX_CM`  | Signaler un poteau à 10 cm avec un signal sonore.     | Action terminée.                            | `SUIVRE_LIGNE`               |
| `CAPTEUR_GAUCHE_ETEINT`   | Ajuster la trajectoire vers la droite.                | Réalignement effectué.                      | `SUIVRE_LIGNE`               |
| `CAPTEUR_DROIT_ETEINT`    | Ajuster la trajectoire vers la gauche.                | Réalignement effectué.                      | `SUIVRE_LIGNE`               |
| `EXTREMITE_TROUVER`       | Signaler l'extrémité et arrêter.                      | Aucun (fin de l'algorithme).                | -                            |

*/
#ifndef TROUVER_POTEAU_H
#define TROUVER_POTEAU_H

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"
#include "capteurLigne.h"
#include "capteurDistance.h"
#include "roue.h"
#include "del.h"
#include "musique.h"
#include "usart.h"
#include "debug.h"
#include "rapport.h"
#include "bouton.h"

namespace TrouverPoteauEtats {
    enum EtatRobot {
        ETAT_INITIAL,
        SUIVRE_LIGNE,
        VERIFIER_CAPTEURS_ALLUME,
        VERIFIER_PRESENCE_POTEAU,
        TROUVER_LIGNE,
        POTEAU_DETECTER_DIX_CM,
        VERIFIER_EXTREMITE_GAUCHE,
        VERIFIER_EXTREMITE_DROITE,
        CAPTEUR_GAUCHE_ETEINT,
        CAPTEUR_DROIT_ETEINT,
        EXTREMITE_TROUVER
    };
}

class TrouverPoteau {
public:
    TrouverPoteau(can* can, CapteurLigne* capteurLigne, CapteurDistance* capteurDistance, 
                   Roue* roue, Del* del, Musique* musique, Rapport* rapport, Bouton* bouton) 
        : can_(can),
          capteurLigne_(capteurLigne),
          capteurDistance_(capteurDistance),
          roue_(roue),
          del_(del),
          musique_(musique),
          rapport_(rapport),
          boutonCarteMere_(bouton),
          actionTournerDroiteEffectuee_(false),
          extremiteTrouvee_(false)
          {}
    
    void demarrer();
    void trouverPoteau();
    void mettreAJourPositionEtPositionPoteau();
    uint8_t verifierPresencePoteau();

    void trouverLigne();
    void jouerDeuxSonsAigus();
    void jouerHuitSonsGraves();

    static void gererInterruption();
    static void reinitialiserBoutonRelacheDetecte();
    
private:
    can* can_;
    CapteurLigne* capteurLigne_;
    CapteurDistance* capteurDistance_;
    Roue* roue_;
    Del* del_;
    Musique* musique_;
    Rapport* rapport_;
    Bouton* boutonCarteMere_;

    uint8_t compteurTournerGauche_ = 0;
    uint8_t compteurTournerDroite_ = 0;
    uint8_t compteurCapteurGaucheEteint_ = 0;
    uint8_t compteurCapteurDroitEteint_ = 0;
    uint8_t compteurIntersection_ = 0;
    uint8_t compteurToursApresExtremite  = 0;
    uint8_t compteurPoteauDetecte_ = 0;
    
    bool actionTournerDroiteEffectuee_ = false;
    bool extremiteTrouvee_ = false;

    TrouverPoteauEtats::EtatRobot etatSuivant = TrouverPoteauEtats::ETAT_INITIAL;
    Point pointDepart_ = AUCUN_POINT;
    uint8_t positionPoteau1_ = 0;
    uint8_t positionPoteau2_ = 0;
    uint8_t positionPoteau3_ = 0;

    static volatile uint8_t boutonRelacheDetecte_;


    void reinitialiserAttributs();
};

#endif 