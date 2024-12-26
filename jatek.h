#ifndef JATEK_H
#define JATEK_H

#include "jatekos.h"

//jatekban levo jatekos strukturaja
typedef struct Jatekban{
    char nev[21]; //max 20 karakter a nev + lezaro nulla 
    int nyeremeny;
    int szek;
    int bot;
} Jatekban;

//jatek tomb strukturaja
typedef struct Jatek{
    Jatekban *jatekosok;
    int meret; //jatekosok szama (akar akik nem ulnek asztalnal de mar korabban ultek)
    int botok; //bot nev szamlalo
    int szekek[5]; //melyik szeken ulnek (0 vagy 1)
} Jatek;

Jatekostomb ujjatek(Jatekostomb jatekostomb);

void nevekkiir(Jatekostomb jatekosok);

void removetext(int kezdsor, int vegsor);
#endif