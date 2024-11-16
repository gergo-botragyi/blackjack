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

Asztalnal lapotkap(Asztalnal jatekos, char lapok[14][3]){
    
    int index = (rand()%(13-0+1)+0);
    if(index < 10){
        jatekos.osszeg+=index+1;
    }else if(index != 13){
        jatekos.osszeg+=10;
    }else if(jatekos.osszeg+11>21){
        jatekos.osszeg+=1;
    }else{jatekos.osszeg+=11;}

    strcat(jatekos.lapok, lapok[index]);
    strcat(jatekos.lapok, " ");

    return jatekos;
}

Asztal osztas(Asztal asztal, char lapok[14][3], int leforditott){
    for (int i = 1; i < asztal.meret; i++)
    {
        asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);

        int szek = asztal.jatekosok[i].szek;
        econio_gotoxy(szek*20,5);
        printf("%d", asztal.jatekosok[i].osszeg);

        econio_gotoxy(szek*20,6);
        printf("%s", asztal.jatekosok[i].lapok);

        econio_sleep(1);
    }
    asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok);

    if(!leforditott){
        econio_gotoxy(42,3);
        printf("%d", asztal.jatekosok[0].osszeg);

        econio_gotoxy(42,2);
        printf("%s", asztal.jatekosok[0].lapok);

        econio_sleep(1);
        asztal = osztas(asztal, lapok, 1);
    }
    return asztal;
}

Asztal jatekmenet(Asztal asztal){
    srand(time(NULL));
    char lapok[14][3]  = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    asztal = tetek(asztal);
    asztal = osztas(asztal, lapok, 0);

    for (int i = 1; i < asztal.meret; i++)
    {
        char inp[10];
        
        removetext(12,15);
        lapmenu();
        scanf("%s", inp); 
        if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont

        while(asztal.jatekosok[i].osszeg<21 && inp[0]!=1){
            switch (inp[0])
            {
            case 0:
                asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok); 
                break;
            case 2:
                if(strlen(asztal.jatekosok[i].lapok)==5){
                    asztal.jatekosok[i].tet *= 2;
                    inp[0] = '8';
                }
                break;
            case 3:
                if(strlen(asztal.jatekosok[i].lapok)==5){
                    asztal.jatekosok[i].tet *= -0.5;
                    inp[0] = '8';
                }
            default:
                printf("Nincs ilyen menupont!");
                econio_sleep(3);
                break;
            }

            removetext(16,16);

            if(inp[0] != '8'){
                scanf("%s", inp); 
                if(!szame(inp,1)){inp[0] = '8';}
            }
        }
    }
    
    //nyeremeny osszeget itt kell meghatarozni (tet szorzodik)
    econio_sleep(3);
    return asztal;
}