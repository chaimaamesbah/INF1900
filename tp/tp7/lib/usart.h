/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Il permet de transmettre et de recevoir des données via le protocole USART, 
ainsi que d'envoyer des chaînes de caractères et des données numériques.

Identifications des matérielles : 
- Input :
        - PD0 (RX)
- Output :
        - PD1 (TX)

*/

#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

class Usart {
public:
    Usart();
    ~Usart() {}

    void transmettre(uint8_t donnee);
    void transmettreDonne(const char *message);
    void transmettreDonne(uint8_t donnee);
    int recevoir();
};

#endif