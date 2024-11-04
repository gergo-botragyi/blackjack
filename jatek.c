#include <stdio.h>
#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

typedef struct Jatekban{
    char nev[20]; //21!!!
    int tet;
    char lapok[11]; //maximum kartyak szama egy jatekosnal
    int osszeg;
    int szek;
} Jatekban;

typedef struct Jatek{
    Jatekban *jatekosok;
    int meret;
    int botok;
    int szekek[5];
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
    int szek = -1;
    for (int i = 1; i < jatek.meret; i++)
    {
        szek = jatek.jatekosok[i].szek;
        if(szek != -1){
            econio_gotoxy((szek)*20,5);
            printf("%d", jatek.jatekosok[i].osszeg);

            econio_gotoxy((szek)*20,6);
            printf("%s", jatek.jatekosok[i].lapok);

            econio_gotoxy((szek)*20,8); //max 20 karakter egy nev
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

char* jatekosnev(Jatekostomb jatekostomb, char *nev){
    removetext(12, 18);

    econio_gotoxy(0,12);
    nevekkiir(jatekostomb);
    printf("Jatekos neve: ");

    scanf("%s", nev);

    removetext(12, 12+(jatekostomb.meret));

    return nev;
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
        if(strcmp(jatek.jatekosok[i].nev, nev)==0 && jatek.jatekosok[i].szek != -1){
            return 1;
        }
    }
    return 0;
}

int uresszek(int *szekek){
    for (int i = 0; i < 5; i++)
    {
        if(szekek[i] == 0){
            return i;
        }
    }
    return -1;
}

int asztalnal(int *szekek){
    int foglalt = 0;
    for (int i = 0; i < 5; i++)
    {
        if(szekek[i]!=0){foglalt++;}
    }
    return foglalt;
}

Jatek jatekhozad(Jatek jatek, int hely, char *nev){
    jatek.meret++;
    int meret = jatek.meret;
    jatek.jatekosok = (Jatekban*)realloc(jatek.jatekosok, meret*sizeof(Jatekban));

    strcpy(jatek.jatekosok[meret-1].nev, nev);
    jatek.jatekosok[meret-1].tet = 0;
    jatek.jatekosok[meret-1].osszeg = 0;
    jatek.jatekosok[meret-1].szek = hely;
    jatek.szekek[hely] = 1;
    jatek.jatekosok[meret-1].lapok[0] = '\0';

    return jatek;
}

Jatek leultetes(Jatek jatek, Jatekostomb jatekostomb, int bot){
    char nev[20]; //21-re atirni mindenhol
    int letrehozott = 0;
    removetext(20,20); //Nincs ilyen jatekos miatt
    if(bot){
        sprintf(nev, "Bot%d", ++jatek.botok);
        letrehozott = 1;
    }else{
        strcpy(nev,jatekosnev(jatekostomb, nev));
        letrehozott = letezik(jatekostomb, nev)!=-1;
    }

    int hely = uresszek(jatek.szekek);
    int meret = jatek.meret;
    
    if(letrehozott && !leultetve(jatek, nev) && asztalnal(jatek.szekek) < 5){
        return jatekhozad(jatek, hely, nev);
    }

    if(letrehozott && !jatszik(jatek, nev) && asztalnal(jatek.szekek) < 5){
        for (int i = 0; i < meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.jatekosok[i].szek = hely;
                jatek.szekek[hely] = 1;
            }
        }
        return jatek;
    }

    if(!letrehozott && asztalnal(jatek.szekek) < 5){
        econio_gotoxy(0,12);
        printf("Ilyen jatekos nem letezik. Szeretned letrehozni? \nIgen - 0 \nNem - 1\n");
        int inp;
        scanf("%d", &inp);

        if(inp == 0){
            jatek = jatekhozad(jatek, hely, nev);
        }
        return jatek;
    }else{
        econio_gotoxy(0,20);
        printf("Ez a jatekos mar jatekban van vagy nincs hely az asztalnal!");
        econio_sleep(3);
    }

    return jatek;
}

Jatek felallitas(Jatek jatek){
    removetext(12, 18);

    econio_gotoxy(0,12);
    printf("Jatekos neve: ");

    char nev[20];
    scanf("%s", nev);

    if(jatszik(jatek, nev)){
        for (int i = 0; i < jatek.meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.szekek[jatek.jatekosok[i].szek] = 0;
                jatek.jatekosok[i].szek = -1;
            }
        }
    }else{
        econio_gotoxy(0,20);
        printf("Ez a jatekos nem ul az asztalnal!");
        econio_sleep(3);
    }
    return jatek;
}

Jatekostomb frissjatekosok(Jatekostomb jatekostomb, Jatek jatek){
    for (int i = 1; i < jatek.meret; i++) //1-tol mert az oszto nem kell
    {
        if(letezik(jatekostomb, jatek.jatekosok[i].nev)==-1){ //nem letezik
            Jatekos uj = {{*jatek.jatekosok[i].nev}, 0, 0};
            jatekostomb.meret++;
            jatekostomb.jatekosok = (Jatekos*)realloc(jatekostomb.jatekosok, jatekostomb.meret*sizeof(Jatekos));
            jatekostomb.jatekosok[jatekostomb.meret-1] = uj;
        }
    }
    return jatekostomb;
}

Jatekostomb ujjatek(Jatekostomb jatekostomb){
    econio_clrscr();
    Jatek jatek = {(Jatekban*)malloc(sizeof(Jatekban)), 1, 0, {0}};
    strcpy(jatek.jatekosok[0].nev, "Oszto");
    jatek.jatekosok[0].tet = 0;
    jatek.jatekosok[0].osszeg = 0;
    jatek.jatekosok[0].szek = -1;
    jatek.jatekosok[0].lapok[0] = '\0';

    char inp[10];

    asztal(jatek);
    menu();
    scanf("%s", inp); 
    if(!szame(inp)){inp[0] = '9';} //input vagy nem letezo menupont

    while(inp[0] != '9'){
        switch (inp[0])
        {
        case '0':
            jatek = leultetes(jatek, jatekostomb, 0);
            break;
        case '1':
            jatek = leultetes(jatek, jatekostomb, 1);
            break;
        case '2':
            if(jatek.meret>1){
                jatek = felallitas(jatek);
            }
            break;
        case '3':
            break;
        
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }
        asztal(jatek);
        menu();
        scanf("%s", inp); 
        if(!szame(inp)){inp[0] = '9';} //input vagy nem letezo menupont
        
    }

    jatekostomb = frissjatekosok(jatekostomb,jatek);

    free(jatek.jatekosok);
    return jatekostomb;
}