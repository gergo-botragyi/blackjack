#include <stdio.h>
#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

void menu(){
    printf("Jatekos hozzadasa - 0\n");
    printf("Bot hozzadasa - 1\n");
    printf("Jatekos eltavolitasa - 2\n");
    printf("Bot eltavolitasa - 3\n");
    printf("Jatek kezdese - 4\n");
    printf("Vissza - 9\n");
}

void nevekkiir(Jatekostomb jatekostomb){
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        printf("Nev: %s\n", jatekostomb.jatekosok[i].nev);
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
    removetext(11, 17);

    econio_gotoxy(0,11);
    nevekkiir(jatekostomb);
    printf("Jatekos neve: ");

    char nev[20];
    scanf("%s", nev);

    removetext(11, 11+(jatekostomb.meret));

    return letezik(jatekostomb, nev)!=-1 ? nev : " ";
}

int leultetve(char **jatszok, int jatekosszam, char *nev){
    for (int i = 0; i < jatekosszam; i++)
    {
        if(strcmp(jatszok[i], nev)){
            return 1;
        }
    }
    return 0;
}

Jatekostomb ujjatek(Jatekostomb jatekostomb){
    econio_clrscr();

    int inp;
    econio_gotoxy(25, 0);
    printf("Dealer\n");
    printf("-----------------------------------------------------------\n\n\n\n\n");
    printf("-----------------------------------------------------------\n\n\n\n\n");
    menu();
    scanf("%d", &inp);

    int jatszokszama = 0;
    char **jatszok = (char**)calloc(jatekostomb.meret,sizeof(char*));
    char nev[20];
    while(inp != 9){
        switch (inp)
        {
        case 0:
            removetext(20,20); //Nincs ilyen jatekos miatt
            strcpy(nev,jatekoshozzaad(jatekostomb));
            if(strcmp(nev, " ")!=0 && !leultetve(jatszok, jatszokszama, nev)){
                econio_gotoxy(jatszokszama*20,7); //max 20 karakter egy nev
                printf("%s", nev);
                jatszokszama++;
                jatszok[jatszokszama-1] = nev;
            }else{
                econio_gotoxy(0,20);
                printf("Nincs ilyen nevu jatekos, vagy mar jatekban van!");
            }
            econio_gotoxy(0,11);
            menu();
            scanf("%d", &inp);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }
        
    }

    free(jatszok);
    return jatekostomb;
}