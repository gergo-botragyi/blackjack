#ifndef JATEKMENET_H
#define JATEKMENET_H

typedef struct Asztalnal{
    char nev[21];
    int tet;
    char lapok[13]; //maximum kartyak szama egy jatekosnal
    int osszeg;
    int szek;
    int bot;
} Asztalnal;

typedef struct Asztal{
    Asztalnal *jatekosok;
    int meret;
} Asztal;

Asztal tetek(Asztal asztal);
Asztal jatekmenet(Asztal asztal);

#endif