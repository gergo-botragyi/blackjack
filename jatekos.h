#ifndef JATEKOS_H
#define JATEKOS_H

typedef struct Jatekos{
    char nev[21]; //max 20 karakter egy név
    int nyeremeny;
    int korok;
} Jatekos;

typedef struct Jatekostomb{
    Jatekos *jatekosok;
    int meret;
} Jatekostomb;

Jatekostomb jatekos(Jatekostomb jatekostomb);

void kiir(Jatekostomb jatekostomb);
void filebair(Jatekostomb jatekostomb);
void fileletrehoz();
int letezik(Jatekostomb jatekostomb, char *reginev);
Jatekostomb szerkesztes(Jatekostomb jatekostomb);
Jatekostomb letrehozas(Jatekostomb jatekostomb);
int filemeret();
Jatekostomb beolvas(Jatekostomb jatekostomb);

#endif