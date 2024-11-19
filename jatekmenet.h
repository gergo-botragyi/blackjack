#ifndef JATEKMENET_H
#define JATEKMENET_H

typedef struct Kartya{
    char lap[3];
    int ertek;
    short szin;
} Kartya;

typedef struct Asztalnal{
    char nev[21];
    int tet;
    Kartya lapok[12]; //maximum kartyak szama +1 egy jatekosnal
    short lapszam;
    int laposszeg;
    int szek;
    int bot;
    short vesztett;
} Asztalnal;

typedef struct Asztal{
    Asztalnal *jatekosok;
    int meret;
} Asztal;

Asztal tetek(Asztal asztal);
Asztal jatekmenet(Asztal asztal);

#endif