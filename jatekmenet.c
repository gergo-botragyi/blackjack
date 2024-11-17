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
    printf("%d", jatekos.tet);
}

void lapotkiir(Asztalnal jatekos, int oszto){
    if(oszto){econio_gotoxy(42,3);}else{econio_gotoxy(jatekos.szek*20,5);}
    printf("%d", jatekos.laposszeg);

    if(oszto){econio_gotoxy(42,2);}else{econio_gotoxy(jatekos.szek*20,6);}
    for (int i = 0; i < jatekos.lapszam; i++)
    {
        printf("%s ", jatekos.lapok[i].lap);
    }
}

void vesztett(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("Vesztett!");
}

void nyert(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("Nyert!");
}

Asztal tetek(Asztal asztal){
    for (int i = 1; i < asztal.meret; i++)
    {
        removetext(12,18);
        econio_gotoxy(0,12);
        if(asztal.jatekosok[i].bot != 0){
            //rand() % (max - min + 1) + min -- a minimum maximum ertekek miatt, ezutan 10-zel osztva, hogy valodibb legyen
            asztal.jatekosok[i].tet = rand() % (20000 - 500 + 1) + 500;
            tetkiir(asztal.jatekosok[i]);
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
        {.lap = "4",.ertek=5},{.lap="5",.ertek=5},
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
        char inp[10];

        if(asztal.jatekosok[i].bot > 0 && asztal.jatekosok[i].bot > asztal.jatekosok[i].laposszeg){inp[0] = '0';}
        else if(asztal.jatekosok[i].bot > 0 && asztal.jatekosok[i].bot <= asztal.jatekosok[i].laposszeg){inp[0] = '1';}
        
        if(inp[0]!='0'&&inp[0]!='1'){
            removetext(12,15);
            lapmenu();
            scanf("%s", inp); 
            if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
        }

        while(inp[0]!='1' && asztal.jatekosok[i].laposszeg<21){
            switch (inp[0])
            {
            case '0':
                asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
                lapotkiir(asztal.jatekosok[i], 0);
                if(asztal.jatekosok[i].laposszeg>21){
                    asztal.jatekosok[i].vesztett = 1;
                    vesztett(asztal.jatekosok[i]);
                    inp[0] = '1';
                }
                if(asztal.jatekosok[i].bot != 0){econio_sleep(1);}
                break;
            case '2':
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i].tet *= 2;
                    tetkiir(asztal.jatekosok[i]);
                    inp[0] = '1';
                }
                break;
            case '3':
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i].tet *= -0.5;
                    tetkiir(asztal.jatekosok[i]);
                    asztal.jatekosok[i].vesztett = 1;
                    vesztett(asztal.jatekosok[i]);
                    inp[0] = '1';
                }
            default:
                printf("Nincs ilyen menupont!");
                econio_sleep(3);
                break;
            }

            removetext(16,17);
            lapmenu();

            if(inp[0] != '1' && asztal.jatekosok[i].bot==0){
                scanf("%s", inp); 
                if(!szame(inp,1)){inp[0] = '8';}
            }
        }
        removetext(16,17);
        lapmenu();
    }
    removetext(12,17);

    lapotkiir(asztal.jatekosok[0],1);
    while(asztal.jatekosok[0].bot>asztal.jatekosok[0].laposszeg && asztal.jatekosok[0].laposszeg<21){
        asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok);
        lapotkiir(asztal.jatekosok[0], 1);
    }

    int osztolaposszeg = asztal.jatekosok[0].laposszeg;
    int jatekoslaposszeg;
    for (int i = 1; i < asztal.meret; i++)
    {
        jatekoslaposszeg = asztal.jatekosok[i].laposszeg;
        if(!asztal.jatekosok[i].vesztett){
            if(osztolaposszeg>21 || jatekoslaposszeg > osztolaposszeg){
                asztal.jatekosok[i].tet *= 2;
                nyert(asztal.jatekosok[i]);
            }else if(osztolaposszeg>jatekoslaposszeg){
                asztal.jatekosok[i].tet *= -1;
                vesztett(asztal.jatekosok[i]);
            }else if(jatekoslaposszeg == 21 && osztolaposszeg != 21){
                asztal.jatekosok[i].tet *= 3/2;
                nyert(asztal.jatekosok[i]);
            }else{asztal.jatekosok[i].tet = 0;}
        }
    }
    
    econio_sleep(5);
    return asztal;
}