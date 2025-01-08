/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : Projet final.
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : 
Ce fichier contient la déclaration et l'implémentation des méthodes de la classe `ModeFonctionnement`. 
Cette classe permet de gérer les différents modes de fonctionnement d'un robot en fonction des conditions définies. 
Elle repose sur une mémoire non volatile pour sauvegarder le mode en cours et permet de réagir à un signal de réinitialisation pour récupérer le dernier mode utilisé.

Méthodes :
- `Mode verifierReset()` : Vérifie si un redémarrage a eu lieu et récupère le mode enregistré dans la mémoire.
- `void enregistrerMode(Mode mode)` : Enregistre le mode courant dans la mémoire pour persistance.
- `void lancerCycleSelection()` : Active cycliquement les différents modes disponibles en indiquant leur activation via des signaux DEL.

Identifications matérielles : 
- Input :
    - Signal de réinitialisation détecté via la mémoire (Bouton Reset).
- Output :
    - Mémoire pour la persistance du mode.
    - DEL pour indiquer visuellement le mode en cours d'activation.

*/

#ifndef MODEFONCTIONNEMENT_H
#define MODEFONCTIONNEMENT_H

#define F_CPU 8000000UL
#include "del.h"
#include "memoire_24.h"
#include <avr/io.h>
#include <util/delay.h>

enum class Mode : uint8_t {
    AUCUN_MODE = 0xFF,
    CHERCHER_EXTREMITE = 0x01,
    TRAVERSER_PARCOURS = 0x02,
    AFFICHER_RAPPORT = 0x03 
};

class ModeFonctionnement {
public:
    ModeFonctionnement(Del* del, Memoire24CXXX* memoire) : del_(del), memoire_(memoire), modeEnCours_(Mode::AUCUN_MODE) {}
    
    Mode verifierReset();
    void enregistrerMode(Mode mode = Mode::AUCUN_MODE);
    void lancerCycleSelection();

private:
    Del* del_;
    Memoire24CXXX* memoire_;
    Mode modeEnCours_;
};

#endif