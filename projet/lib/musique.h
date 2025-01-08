/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP9 - Projet initial de système embarqué
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Ce programme implémente une classe Musique permettant de jouer des notes.
Il configure la minuterie interne (timer 0) pour produire des fréquences spécifiques correspondant à des 
notes MIDI, avec des fonctions pour démarrer, arrêter et gérer la durée des notes.

Identifications des matériels :
- Input :
        - Aucune
- Output :
        - PB3 (PWM)
        - PB2 (ground)

*/

#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

class Musique {
public:
    Musique();

    void jouerNote(double note);
    void arreter();

    enum NoteMIDI {
        NOTE_45 = 110, // A2
        NOTE_48 = 131, // C3
        NOTE_50 = 147, // D3
        NOTE_52 = 165, // E3
        NOTE_53 = 175, // F3
        NOTE_55 = 196, // G3
        NOTE_57 = 220, // A3
        NOTE_60 = 262, // C4
        NOTE_62 = 294, // D4
        NOTE_64 = 330, // E4
        NOTE_65 = 349, // F4
        NOTE_67 = 392, // G4
        NOTE_69 = 440, // A4
        NOTE_71 = 494, // B4
        NOTE_72 = 523, // C5
        NOTE_74 = 587, // D5
        NOTE_76 = 659, // E5
        NOTE_77 = 698, // F5
        NOTE_79 = 784, // G5
        NOTE_81 = 880  // A5
    };

private:
    static constexpr uint16_t PRESCALAGE_MINUTERIE = 256;
    static constexpr uint8_t DIVISEUR_FREQUENCE_MINUTERIE = 2;
};