#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "jatek.h"
#include "jatekos.h"
#include "main.h"
#include "jatekmenet.h"
#include "econio.h"
#include "debugmalloc.h"

Jatek tetek(Jatek jatek){
    for (int i = 0; i < jatek.meret; i++)
    {
        removetext(12,18);
        econio_gotoxy(0,12);
        if(jatek.jatekosok[i].bot){
            //rand() % (max - min + 1) + min -- a minimum maximum ertekek miatt, ezutan 10-zel osztva, hogy valodibb legyen
            jatek.jatekosok[i].tet = rand() % (20000 - 500 + 1) + 500;
            econio_gotoxy((jatek.jatekosok[i].szek)*20,9);
            printf("%d", jatek.jatekosok[i].tet);
        }
        else if(jatek.jatekosok[i].szek != -1){
            char tet[21];
            do
            {
                printf("Jatekos: %s\n", jatek.jatekosok[i].nev);
                printf("Tet: ");
                scanf("%s", tet);
            } while (!szame(tet, 0));
            jatek.jatekosok[i].tet = atoi(tet); //ascii kodok miatt
            econio_gotoxy((jatek.jatekosok[i].szek)*20,9);
            printf("%d", jatek.jatekosok[i].tet);
        }
    }
    return jatek;
}

Jatek jatekmenet(Jatek jatek){
    srand(time(NULL));
    jatek = tetek(jatek);
    econio_sleep(3);
    return jatek;
}