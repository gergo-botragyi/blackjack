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

void tetkiir(Asztalnal jatekos){
    econio_gotoxy((jatekos.szek)*20,9);
    printf("%s%d%s","\033[0;33m", jatekos.tet,"\033[0m");
}

void lapotkiir(Asztalnal jatekos, int oszto){
    if(oszto){econio_gotoxy(48,3);}else{econio_gotoxy(jatekos.szek*20,5);}
    econio_textbackground(COL_GREEN);
    printf("%s[%d]%s", "\033[1;93m", jatekos.laposszeg,"\033[0m");
    
    if(oszto){econio_gotoxy(48,2);}else{econio_gotoxy(jatekos.szek*20,6);}
    for (int i = 0; i < jatekos.lapszam; i++)
    {
        if(jatekos.lapok[i].szin){
            printf("\033[42m\033[1;96m"); //cyan
        }else{printf("\033[42m\033[1;91m");} //red
        printf("%s ", jatekos.lapok[i].lap);
    }
    econio_textcolor(COL_LIGHTGRAY);
    econio_textbackground(COL_BLACK);
}

void vesztett(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sVesztett!%s","\033[31m","\033[0m");
}

void nyert(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sNyert!%s","\033[32m","\033[0m");
}

void dontetlen(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sDontetlen!%s","\033[34m","\033[0m");
}

Asztal tetek(Asztal asztal){
    for (int i = 1; i < asztal.meret; i++)
    {
        removetext(12,18);
        econio_gotoxy(0,12);
        if(asztal.jatekosok[i].bot != 0){
            econio_sleep(1);
            //rand() % (max - min + 1) + min -- a minimum maximum ertekek miatt, ezutan 10-zel osztva, hogy valodibb legyen
            asztal.jatekosok[i].tet = rand() % (5000 - 500 + 1) + 500;
            tetkiir(asztal.jatekosok[i]);
        }
        else{
            char tet[21];
            do
            {
                printf("Jatekos: %s%s%s\n","\033[0;32m", asztal.jatekosok[i].nev,"\033[0m");
                printf("Tet: ");
                printf("\033[33m");
                scanf("%20s", tet);
                econio_textcolor(COL_LIGHTGRAY);
                char extra = 0;
                while((extra=getchar())!='\n' && extra!=EOF); //20. beolvasott karakter utan levo karakterek ott maradnanak a bemeneten
            } while (!szame(tet, 0));
            asztal.jatekosok[i].tet = atoi(tet); //ascii kodok miatt
            tetkiir(asztal.jatekosok[i]);
        }
    }
    return asztal;
}

Asztalnal lapotkap(Asztalnal jatekos, Kartya *lapok){
    
    int index = (rand()%(12-0+1)+0);
    int lapertek = lapok[index].ertek;
    if(lapertek+jatekos.laposszeg>21){
        int i = 0;
        while(i<jatekos.lapszam && jatekos.laposszeg+lapertek>21){
            if(jatekos.lapok[i].ertek==11 && jatekos.laposszeg-10+lapertek<=21){
                jatekos.lapok[i].ertek = 1;
                jatekos.laposszeg -= 10;
            }
            i++;
        }
    }

    jatekos.lapok[jatekos.lapszam] = lapok[index];
    jatekos.lapok[jatekos.lapszam].szin = (rand()%(1-0+1)+0);
    jatekos.laposszeg+=lapertek; 
    if(lapertek==11 && jatekos.laposszeg>21){
        jatekos.laposszeg -= 10;
        jatekos.lapok[jatekos.lapszam].ertek = 1; 
    }
    jatekos.lapszam++;

    return jatekos;
}


Asztal osztas(Asztal asztal, Kartya *lapok, int leforditott){
    for (int i = 1; i < asztal.meret; i++)
    {
        asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
        lapotkiir(asztal.jatekosok[i], 0);
        econio_sleep(1);
    }
    asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok);

    if(!leforditott){
        lapotkiir(asztal.jatekosok[0],1);
        econio_sleep(1);
        asztal = osztas(asztal, lapok, 1);
    }
    return asztal;
}

Asztal jatekmenet(Asztal asztal){
    srand(time(NULL));

    Kartya lapok[] = {
        {.lap = "2",.ertek=2},{.lap="3",.ertek=3},
        {.lap = "4",.ertek=4},{.lap="5",.ertek=5},
        {.lap = "6",.ertek=6},{.lap="7",.ertek=7},
        {.lap = "8",.ertek=8},{.lap="9",.ertek=9},
        {.lap = "10",.ertek=10},{.lap="J",.ertek=10},
        {.lap = "Q",.ertek=10},{.lap="K",.ertek=10},
        {.lap = "A",.ertek=11}
    };

    asztal = tetek(asztal);
    asztal = osztas(asztal, lapok, 0);

    for (int i = 1; i < asztal.meret; i++)
    {
        int inp = 8;
        int botlep = asztal.jatekosok[i].bot > asztal.jatekosok[i].laposszeg;

        if(botlep){inp = 0; botnev(asztal.jatekosok[i]); econio_sleep(1);}
        else if(asztal.jatekosok[i].bot > 0 && !botlep){inp = 1; botnev(asztal.jatekosok[i]); econio_sleep(1);}
        
        if(asztal.jatekosok[i].bot == 0 && asztal.jatekosok[i].laposszeg < 21){
            removetext(12,16);
            lapmenu(asztal.jatekosok[i]);
            inp = input();
        }

        while(inp!=1 && asztal.jatekosok[i].laposszeg<21){
            switch (inp)
            {
            case 0:
                asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
                lapotkiir(asztal.jatekosok[i], 0);
                if(asztal.jatekosok[i].laposszeg>21){
                    asztal.jatekosok[i].vesztett = 1;
                    asztal.jatekosok[i].tet *= -1;
                    vesztett(asztal.jatekosok[i]);
                    inp = 1;
                }
                if(asztal.jatekosok[i].bot > 0){econio_sleep(1);}
                break;
            case 2:
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i].tet *= 2;
                    tetkiir(asztal.jatekosok[i]);
                    inp = 1;
                }
                break;
            case 3:
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i].tet *= -0.5;
                    tetkiir(asztal.jatekosok[i]);
                    asztal.jatekosok[i].vesztett = 1;
                    vesztett(asztal.jatekosok[i]);
                    inp = 1;
                }
                break;
            default:
                printf("%sNincs ilyen menupont!%s","\033[31m","\033[0m");
                econio_sleep(3);
                break;
            }
            botlep = asztal.jatekosok[i].bot > asztal.jatekosok[i].laposszeg;

            removetext(17,18);
            lapmenu(asztal.jatekosok[i]);

            if(inp != 1 && asztal.jatekosok[i].laposszeg<21 &&asztal.jatekosok[i].bot==0){
                inp = input();
            }else if(botlep && asztal.jatekosok[i].bot>0){
                inp = 0;
            }else{
                inp = 1;
            }
        }
        removetext(12,18);
    }
    removetext(12,18);

    lapotkiir(asztal.jatekosok[0],1);
    econio_sleep(1);
    while(asztal.jatekosok[0].bot>asztal.jatekosok[0].laposszeg && asztal.jatekosok[0].laposszeg<21){
        asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok);
        lapotkiir(asztal.jatekosok[0], 1);
        econio_sleep(1);
    }

    int osztolaposszeg = asztal.jatekosok[0].laposszeg;
    int jatekoslaposszeg;
    for (int i = 1; i < asztal.meret; i++)
    {
        jatekoslaposszeg = asztal.jatekosok[i].laposszeg;
        if(!asztal.jatekosok[i].vesztett){
            if(jatekoslaposszeg == 21 && osztolaposszeg != 21){
                asztal.jatekosok[i].tet /= 2;
                asztal.jatekosok[i].tet *= 3;
                nyert(asztal.jatekosok[i]);
            }else if(osztolaposszeg>21 || jatekoslaposszeg > osztolaposszeg){
                asztal.jatekosok[i].tet *= 2;
                nyert(asztal.jatekosok[i]);
            }else if(osztolaposszeg>jatekoslaposszeg){
                asztal.jatekosok[i].tet *= -1;
                vesztett(asztal.jatekosok[i]);
            }else{
                asztal.jatekosok[i].tet = 0;
                dontetlen(asztal.jatekosok[i]);
            }
        }
    }
    
    econio_gotoxy(0,18);
    printf("%sNyomj entert a tovabblepeshez!%s","\033[33m","\033[0m");
    getchar();
    return asztal;
}