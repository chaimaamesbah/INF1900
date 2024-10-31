/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Il contient les definitions des fonctions debugPrintNum et debugPrintChar, qui sont 
                            declaree et associees aux macros DEBUG_PRINT_NUM(x) et DEBUG_PRINT_CHAR(x),
                            respectivement, dans le fichier debug.h

Identifications des matérielles : Il n'y a pas d'identification materielle specifique a ce programme
*/

#include "debug.h"

//template<typename T>

void debugPrintNum(uint8_t donnee){

    Usart usart=Usart();
    usart.transmettreDonne(donnee);
}


void debugPrintChar(const char *message){

    Usart usart=Usart();
    usart.transmettreDonne(message);
}
