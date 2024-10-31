/*
Noms des auteurs : ABDEJALIL Imane (2240526), MESBAH Chaïmaa (2342801), SABOURI Asmaa (2224168) et DAKIR Sara (2254926).
Travail : TP7 - Mise en commun du code et formation de librairies
Section # : SECTION 01
Équipe # : EQUIPE_0314

Description du programme : Il contient les declaration des macros DEBUG_PRINT_NUM(x) et DEBUG_PRINT_CHAR(x) qui 
                            aident au deboggage en permettant d'afficher la valeur de x dans le terminal, que ce 
                            soit une donnee numerique ou une chaine de caracteres.

Identifications des matérielles : Il n'y a pas d'identification materielle specifique a ce programme.
*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdint.h>
#include "usart.h"


void debugPrintNum(uint8_t donnee);
void debugPrintChar(const char *message);


#ifdef DEBUG

#define DEBUG_PRINT_NUM(x)  debugPrintNum(x)
#define DEBUG_PRINT_CHAR(x)  debugPrintChar(x)
#else
#define DEBUG_PRINT_NUM(x) do {} while (false)
# define DEBUG_PRINT_CHAR(x) do {} while (false)

#endif
