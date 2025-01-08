/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier contient l'implémentation des méthodes de la classe `CapteurLigne`. 
Cette classe permet de gérer les capteurs utilisés pour suivre une ligne. Elle offre des fonctionnalités 
pour lire les états des capteurs, ajuster la trajectoire du robot en fonction de ces états, et détecter 
les extrémités ou intersections du parcours.

Méthodes :
- `void mettreAJourCapteurs()` : Met à jour l'état des capteurs.
- `uint8_t obtenirNombreCapteursAllumes()` : Retourne le nombre de capteurs actuellement actifs.
- `bool estUneLigne()` : Vérifie si une ligne est détectée sous les capteurs.
- `bool estCinqCapteursAllumes()` : Vérifie si tous les capteurs sont actifs.
- `bool estTousCapteursEteints()` : Vérifie si aucun capteur n'est actif.
- `void avancerEnSuivantLigne(uint8_t vitesse)` : Ajuste la trajectoire pour suivre une ligne en fonction des capteurs.
- `void verifierExtremiteGauche(bool &estLigneTrouverGauche)` : Vérifie la présence d'une extrémité à gauche.
- `void verifierExtremiteDroite(bool &estLigneTrouverDroite)` : Vérifie la présence d'une extrémité à droite.
- `void ajusterTrajectoire(bool estDirectionDroite)` : Ajuste la trajectoire pour réaligner le robot sur une ligne.

Identifications matérielles : 
- Input :
    - Capteur a l'extrémité gauche = PC7
    - Capteur gauche-milieu        = PC6
    - Capteur milieu               = PC5
    - Capteur droite-milieu        = PC4
    - Capteur droite               = PC3
- Output :
    - Contrôle des roues du robot en fonction des états des capteurs.

*/

#ifndef CAPTEURLIGNE_H
#define CAPTEURLIGNE_H

#define F_CPU 8000000UL
#include "debug.h"
#include "roue.h"
#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>

class CapteurLigne {
public:
    CapteurLigne(Roue *roue);

    void mettreAJourCapteurs();
    uint8_t obtenirNombreCapteursAllumes();

    bool estUneLigne();
    bool estSeulementMilieuActif();
    bool estCinqCapteursAllumes();
    bool estTroisCapteursDroiteActif();
    bool estTroisCapteursGaucheActif();
    bool estTousCapteursEteints();

    void avancerEnSuivantLigne(uint8_t vitesse = 40);
    void verifierCapteursAllume(uint8_t &maxCapteursAllumes_,
                                uint8_t &minCapteursAllumes_,
                                bool &estGaucheActif, bool &estDroiteActif);
    void verifierExtremiteGauche(bool &estLigneTrouverGauche);
    void verifierExtremiteDroite(bool &estLigneTrouverDroite);
    void remettreMilieu(bool estDirectionDroite);
    void ajusterTrajectoire(bool estDirectionDroite);
    void tournerPourTrouverLigne(bool estDirectionGauche);
    void tournerDroiteApresPointSix();
    void replacerCorrectementExtremite();

    static bool estGaucheActif;
    static bool estGaucheMilieuActif;
    static bool estMilieuActif;
    static bool estDroiteMilieuActif;
    static bool estDroiteActif;

private:
    Roue *roue_;

    static constexpr uint8_t MASQUE_GAUCHE = (1 << PC7);
    static constexpr uint8_t MASQUE_GAUCHE_MILIEU = (1 << PC6);
    static constexpr uint8_t MASQUE_MILIEU = (1 << PC5);
    static constexpr uint8_t MASQUE_DROITE_MILIEU = (1 << PC4);
    static constexpr uint8_t MASQUE_DROITE = (1 << PC3);
};

#endif