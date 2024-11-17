#include <stdio.h>

#include "econio.h"
#include "debugmalloc.h"
#include "jatek.h"
#include "jatekmenet.h"
#include "jatekos.h"
#include "main.h"

//jatekos
//jatekos menu kiirasa
void jatekosmenu(){
    printf("Jatekosok szerkesztese - 0\n");
    printf("Jatekosok letrehozasa - 1\n");
    printf("Vissza - 9\n");
}

//main
//menu kiirasa
void mainmenu(){
    printf("Uj jatek - 0\n");
    printf("Jatekosok - 1\n");
    printf("GYIK - 2\n");
    printf("Kilepes - 9\n");
}

//jatek
//menu kiirasa
void jatekmenu(){
    printf("Jatekos hozzadasa - 0\n");
    printf("Bot hozzadasa - 1\n");
    printf("Jatekos/Bot eltavolitasa - 2\n");
    printf("Jatek kezdese - 3\n");
    printf("Vissza - 9\n");
}

//jatekmenet
//asztalra a jatekosok adatainak kiirasa (bemenet az asztalnal jatekban levo jatekosok)
/*void asztaladatok(Asztalnal *asztal){
    econio_gotoxy(44, 2); //az oszto lapjait kozepre kell kiirni
    printf("%s\n", asztal[0].lapok);

    econio_gotoxy(44, 3); //az oszto osszege is kozepre megy
    if(asztal[0].osszeg != 0){
        printf("%d", asztal[0].osszeg); 
    }

    int szek = -1; //az adott jatekos szeke, hogy ne kelljen folyton lekerni (-1-re inicializalva biztonsagbol)
    for (int i = 1; i < 6; i++)//minden jatekos lapjai lapjainak osszege es tetje kiirasa
    {
        szek = asztal[i].szek;
        if(szek != -1){
            econio_gotoxy((szek)*20,5); //a szeke sorszama*20 karakterrel toljuk jobbra, mivel 20 karakter max egy nev
            if(asztal[i].osszeg != 0){
                printf("%d", asztal[i].osszeg);
            }

            econio_gotoxy((szek)*20,6);
            printf("%s", asztal[i].lapok);

            econio_gotoxy((szek)*20,9);
            if(asztal[i].tet != 0){
                printf("%d", asztal[i].tet);
            }
        }
    }
}*/

//jatek
//asztal rajzanak kiirasa (nevekkel)
//bemenet a jatek tomb
void asztalrajz(Jatek jatek){
    econio_clrscr();

    econio_gotoxy(42, 0); //elso sorban az asztal kozepere irjuk ki az osztot
    printf("Oszto\n");
    printf("------------------------------------------------------------------------------------------\n");
    printf("\n\n\n\n\n"); //a sok uj sor az asztal helye (merete)

    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 1; i < jatek.meret; i++)
    {
        if(jatek.jatekosok[i].szek != -1){
            econio_gotoxy((jatek.jatekosok[i].szek)*20,8);
            printf("%s", jatek.jatekosok[i].nev);
        }
    }
    
    printf("\n\n\n\n"); //asztal alatti terulet
}

void lapmenu(){
    econio_gotoxy(0,12);
    printf("Hit - 0\n");
    printf("Stand - 1\n");
    printf("Double - 2\n");
    printf("Surrender - 3\n");
}