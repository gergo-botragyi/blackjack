#include <stdio.h>
#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

typedef struct Jatekban{
    char nev[20]; //21!!!
    int tet;
    char lapok[11]; //maximum kartyak szama egy jatekosnal
    int osszeg;
    int jatszik;
} Jatekban;

typedef struct Jatek{
    Jatekban *jatekosok;
    int meret;
    int botok;
} Jatek;

typedef struct Kartya{
    char lap[2];
    int db;
} Kartya;

void menu(){
    printf("Jatekos hozzadasa - 0\n");
    printf("Bot hozzadasa - 1\n");
    printf("Jatekos/Bot eltavolitasa - 2\n");
    printf("Jatek kezdese - 3\n");
    printf("Vissza - 9\n");
}

void asztal(Jatek jatek){
    econio_clrscr();

    econio_gotoxy(42, 0);
    printf("Dealer\n");
    printf("------------------------------------------------------------------------------------------\n");
    econio_gotoxy(44, 2);
    printf("%s\n", jatek.jatekosok[0].lapok);
    econio_gotoxy(44, 3);
    printf("%d\n\n\n\n", jatek.jatekosok[0].osszeg);

    printf("------------------------------------------------------------------------------------------\n");

    int j = 0;
    for (int i = 1; i < jatek.meret; i++)
    {
        if(jatek.jatekosok[i].jatszik){
            econio_gotoxy((j)*20,5);
            printf("%d", jatek.jatekosok[i].osszeg);

            econio_gotoxy((j)*20,6);
            printf("%s", jatek.jatekosok[i].lapok);

            econio_gotoxy((j)*20,8); //max 20 karakter egy nev
            printf("%s", jatek.jatekosok[i].nev);
            j++;
        }
    }
    printf("\n\n\n\n");
}

void nevekkiir(Jatekostomb jatekostomb){
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        printf("Jatekos: %s\n", jatekostomb.jatekosok[i].nev);
    }
}

void removetext(int kezdsor, int vegsor){
    for (int i = kezdsor; i <= vegsor; i++)
    {
        econio_gotoxy(0,i);
        printf("%50s", "");
    }
}

char* jatekoshozzaad(Jatekostomb jatekostomb){
    removetext(12, 18);

    econio_gotoxy(0,12);
    nevekkiir(jatekostomb);
    printf("Jatekos neve: ");

    char nev[20];
    scanf("%s", nev);

    removetext(12, 12+(jatekostomb.meret));

    return letezik(jatekostomb, nev)!=-1 ? nev : "#";
}

int leultetve(Jatek jatek, char *nev){
    for (int i = 0; i < jatek.meret; i++)
    {
        if(strcmp(jatek.jatekosok[i].nev, nev)==0){
            return 1;
        }
    }
    return 0;
}

int jatszik(Jatek jatek, char *nev){
    for (int i = 0; i < jatek.meret; i++)
    {
        if(strcmp(jatek.jatekosok[i].nev, nev)==0 && jatek.jatekosok[i].jatszik){
            return 1;
        }
    }
    return 0;
}

Jatek leultetes(Jatek jatek, Jatekostomb jatekostomb, int bot){
    char nev[20]; //21-re atirni mindenhol
    removetext(20,20); //Nincs ilyen jatekos miatt
    if(bot){
        sprintf(nev, "Bot%d", ++jatek.botok);
    }else{
        strcpy(nev,jatekoshozzaad(jatekostomb)); //# ha nem letezik
    }

    if(strcmp(nev, "#")!=0 && !leultetve(jatek, nev) && jatek.meret < 6){
        //asztal(jatek);

        jatek.meret++;
        jatek.jatekosok = (Jatekban*)realloc(jatek.jatekosok, jatek.meret*sizeof(Jatekban));

        strcpy(jatek.jatekosok[jatek.meret-1].nev, nev);
        jatek.jatekosok[jatek.meret-1].tet = 0;
        jatek.jatekosok[jatek.meret-1].osszeg = 0;
        jatek.jatekosok[jatek.meret-1].jatszik = 1;
        jatek.jatekosok[jatek.meret-1].lapok[0] = '\0';
    }else if(strcmp(nev, "#")!=0 && !jatszik(jatek, nev) && jatek.meret < 6){
        for (int i = 0; i < jatek.meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.jatekosok[i].jatszik = 1;
            }
        }
    }else{
        econio_gotoxy(0,20);
        printf("Nem letezo/mar jatekban levo jatekos vagy nincs hely az asztalnal!");
        econio_sleep(3);
    }

    return jatek;
}

Jatek felallitas(Jatek jatek){ //todo: sorrend csere
    removetext(12, 18);

    econio_gotoxy(0,12);
    printf("Jatekos neve: ");

    char nev[20];
    scanf("%s", nev);

    if(jatszik(jatek, nev)){
        for (int i = 0; i < jatek.meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.jatekosok[i].jatszik = 0;
            }
        }
    }else{
        econio_gotoxy(0,20);
        printf("Ez a jatekos nem ul az asztalnal!");
        econio_sleep(3);
    }
    return jatek;
}

Jatekostomb ujjatek(Jatekostomb jatekostomb){
    econio_clrscr();
    Jatek jatek = {(Jatekban*)malloc(sizeof(Jatekban)), 1, 0};
    strcpy(jatek.jatekosok[0].nev, "Oszto");
    jatek.jatekosok[0].tet = 0;
    jatek.jatekosok[0].osszeg = 0;
    jatek.jatekosok[0].jatszik = 1;
    jatek.jatekosok[0].lapok[0] = '\0';

    int inp;
    asztal(jatek);
    menu();
    scanf("%d", &inp);

    while(inp != 9){
        switch (inp)
        {
        case 0:
            jatek = leultetes(jatek, jatekostomb, 0);
            asztal(jatek);
            menu();
            scanf("%d", &inp);
            break;
        case 1:
            jatek = leultetes(jatek, jatekostomb, 1);
            asztal(jatek);
            menu();
            scanf("%d", &inp);
            break;
        case 2:
            if(jatek.meret>1){
                jatek = felallitas(jatek);
            }
            asztal(jatek);
            menu();
            scanf("%d", &inp);
            break;
        case 3:
            break;
        
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }
        
    }

    free(jatek.jatekosok);
    return jatekostomb;
}