#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "jatek.h"
#include "jatekos.h"
#include "main.h"
#include "jatekmenet.h"
#include "econio.h"
#include "debugmalloc.h"
#include "konzol.h"

/*
typedef struct Kartya{
    char lap[2];
    int db;
} Kartya;*/

Asztal tetek(Asztal asztal){
    for (int i = 1; i < asztal.meret; i++)
    {
        removetext(12,18);
        econio_gotoxy(0,12);
        if(asztal.jatekosok[i].bot != 0){
            //rand() % (max - min + 1) + min -- a minimum maximum ertekek miatt, ezutan 10-zel osztva, hogy valodibb legyen
            asztal.jatekosok[i].tet = rand() % (20000 - 500 + 1) + 500;
            econio_gotoxy((asztal.jatekosok[i].szek)*20,9);
            printf("%d", asztal.jatekosok[i].tet);
        }
        else{
            char tet[21];
            do
            {
                printf("Jatekos: %s\n", asztal.jatekosok[i].nev);
                printf("Tet: ");
                scanf("%s", tet);
            } while (!szame(tet, 0));
            asztal.jatekosok[i].tet = atoi(tet); //ascii kodok miatt
            econio_gotoxy((asztal.jatekosok[i].szek)*20,9);
            printf("%d", asztal.jatekosok[i].tet);
        }
    }
    return asztal;
}

Asztal jatekmenet(Asztal asztal){
    srand(time(NULL));
    asztal = tetek(asztal);
    //nyeremeny osszeget itt kell meghatarozni (tet szorzodik)
    econio_sleep(3);
    return asztal;
}