#include <stdio.h>

#include "jatek.h"
#include "jatekos.h"
#include "main.h"
#include "econio.h"
#include "debugmalloc.h"

//jatekban levo jatekos strukturaja
typedef struct Jatekban{
    char nev[21]; //max 20 karakter a nev + lezaro nulla
    int tet;
    char lapok[11]; //maximum kartyak szama egy jatekosnal
    int osszeg;
    int szek;
} Jatekban;

//jatek tomb strukturaja
typedef struct Jatek{
    Jatekban *jatekosok;
    int meret; //jatekosok szama (akar akik nem ulnek asztalnal de mar korabban ultek)
    int botok; //bot nev szamlalo
    int szekek[5]; //melyik szeken ulnek (0 vagy 1)
} Jatek;

/*
typedef struct Kartya{
    char lap[2];
    int db;
} Kartya;*/

//menu kiirasa
void menu(){
    printf("Jatekos hozzadasa - 0\n");
    printf("Bot hozzadasa - 1\n");
    printf("Jatekos/Bot eltavolitasa - 2\n");
    printf("Jatek kezdese - 3\n");
    printf("Vissza - 9\n");
}

//asztal kiirasa
//bemenet a jatek tomb
void asztal(Jatek jatek){
    econio_clrscr();

    econio_gotoxy(42, 0); //elso sorban az asztal kozepere irjuk ki az osztot
    printf("Dealer\n");
    printf("------------------------------------------------------------------------------------------\n");
    econio_gotoxy(44, 2); //az oszto lapjait is kozepre kell kiirni
    printf("%s\n", jatek.jatekosok[0].lapok);
    econio_gotoxy(44, 3); //az oszto osszege is kozepre megy
    printf("%d\n\n\n\n", jatek.jatekosok[0].osszeg); //a sok uj sor az asztal helye (merete)

    printf("------------------------------------------------------------------------------------------\n");

    int szek = -1; //az adott jatekos szeke, hogy ne kelljen folyton lekerni (-1-re inicializalva biztonsagbol)
    for (int i = 1; i < jatek.meret; i++)//minden jatekos neve, lapjai es lapjainak osszegenek kiirasa
    {
        szek = jatek.jatekosok[i].szek;
        if(szek != -1){
            econio_gotoxy((szek)*20,5); //a szeke sorszama*20 karakterrel toljuk jobbra, mivel 20 karakter max egy nev
            printf("%d", jatek.jatekosok[i].osszeg);

            econio_gotoxy((szek)*20,6);
            printf("%s", jatek.jatekosok[i].lapok);

            econio_gotoxy((szek)*20,8);
            printf("%s", jatek.jatekosok[i].nev);
        }
    }
    printf("\n\n\n\n"); //asztal alatti terulet
}

//kiirja a mentett neveket a kepernyore
//bemenet az eddig mentett jatekostomb (ami ugye a fajlban levo neveket ismeri)
void nevekkiir(Jatekostomb jatekostomb){
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        printf("Jatekos: %s\n", jatekostomb.jatekosok[i].nev);
    }
}

//egy adott sortol egy masik adott sorig torli a konzolt
void removetext(int kezdsor, int vegsor){
    for (int i = kezdsor; i <= vegsor; i++)
    {
        econio_gotoxy(0,i);
        printf("%50s", "");
    }
}

//jatekos valaszto
//bemenet a fajlba mentett jatekosokat ismero jatekostomb es egy nev (ures sztring)
//kiirja a jatekosok neveit, majd beker egy nevet amit aztan visszaad
char* jatekosnev(Jatekostomb jatekostomb, char *nev){
    removetext(12, 18);

    econio_gotoxy(0,12);
    nevekkiir(jatekostomb);
    printf("Jatekos neve: ");

    scanf("%s", nev);

    removetext(12, 12+(jatekostomb.meret));

    return nev;
}

//megnezi le volt-e ultetve egy adott jatekos
//bemenet a jatek tomb es egy nev
//vegigmegy a tombon es ha talal olyan nevu jatekost akkor 1-et ad vissza, amugy 0-t
int voltleultetve(Jatek jatek, char *nev){
    for (int i = 0; i < jatek.meret; i++)
    {
        if(strcmp(jatek.jatekosok[i].nev, nev)==0){
            return 1;
        }
    }
    return 0;
}

//megnezi egy adott nevu jatekos jatszik-e (asztalnal ul)
//bemenet a jtek tomb es egy nev
//ha talal olyan nevu jatekost es annak a jatekosnak a szekszama nem -1 akkor 1-et ad vissza, amugy 0-t
int jatszik(Jatek jatek, char *nev){
    for (int i = 0; i < jatek.meret; i++)
    {
        if(strcmp(jatek.jatekosok[i].nev, nev)==0 && jatek.jatekosok[i].szek != -1){
            return 1;
        }
    }
    return 0;
}

//visszaadja a legelso ures szeket
//bemenet egy int tomb ami 0-kat es 1-eseket tartalmaz
//egy szek ures ha 0
//ha nincs ilyen szek -1-et ad vissza
int uresszek(int *szekek){
    for (int i = 0; i < 5; i++)
    {
        if(szekek[i] == 0){
            return i;
        }
    }
    return -1;
}

//visszaadja hanyan ulnek az asztalnal
//bemenet egy int tomb (0 vagy 1 minden elem)
//ahany 1-est szamol, annyian ulnek az asztalnal
//visszaad egy szamot, ami lehet 0 is
int asztalnal(int *szekek){
    int foglalt = 0;
    for (int i = 0; i < 5; i++)
    {
        if(szekek[i]==1){foglalt++;}
    }
    return foglalt;
}

//hozzaad egy uj jatekost a jatek tombhoz
//bemenet a jatek tomb, a hely ahova leul a jatekos es a neve
//noveli a jatekosok tombjet es inicializalja a jatekost az utolso helyre
//visszaadja a jatek tombot
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

//a jatekosok leulteteseert felelos function
//bemenet a jatek tomb, az addig mentett jatekostomb es, hogy bot-ot akarunk e leultetni vagy sem (0 vagy 1)
//ha bot, letrehozza a nevet a bot szamlalo alapjan
//ha nem bot akkor bekeri a nevet
Jatek leultetes(Jatek jatek, Jatekostomb jatekostomb, int bot){
    char nev[21];
    int letrehozott = 0;
    removetext(20,20); //jatekban van vagy nincs hely az asztalnal kiiras eltuntetese (20. sor)
    if(bot){
        sprintf(nev, "Bot%d", ++jatek.botok);
        letrehozott = 1;
    }else{
        strcpy(nev,jatekosnev(jatekostomb, nev));
        letrehozott = letezik(jatekostomb, nev)!=-1; //megnezi, hogy korabban volt e mar mentve (fajlba irva) a jatekos neve
    }

    int hely = uresszek(jatek.szekek);
    int meret = jatek.meret;
    
    if(letrehozott && !voltleultetve(jatek, nev) && asztalnal(jatek.szekek) < 5){ //ha mar ismeri a nevet de nem volt leultetve es van hely az asztalnal
        return jatekhozad(jatek, hely, nev);
    }

    if(letrehozott && !jatszik(jatek, nev) && asztalnal(jatek.szekek) < 5){ //ismeri a nevet, volt mar asztalnal de nem jatszik jelenleg es van hely
        for (int i = 0; i < meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.jatekosok[i].szek = hely;
                jatek.szekek[hely] = 1;
            }
        }
        return jatek;
    }

    if(!letrehozott && asztalnal(jatek.szekek) < 5){ //ha egy vadiuj jatekost akarna leultetni
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


//jatekos asztaltol valo felallitasaert felelos function
//bemenet a jatek tomb, kimenet a modositott jatek tomb
Jatek felallitas(Jatek jatek){
    removetext(12, 18); //a menu-t torli

    econio_gotoxy(0,12);
    printf("Jatekos neve: ");

    char nev[21];
    scanf("%s", nev);

    if(jatszik(jatek, nev)){ //ha jatszik akkor -1 a jatekos szeke, az adott szek pedig 0 mert ures lesz
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

//vadiuj jatekosok mentese a jatekostombbe
//a jatek soran uj jatekosokat menti a jatekostombbe, hogy azt ki tudja irni fajlba es menteni
Jatekostomb frissjatekosok(Jatekostomb jatekostomb, Jatek jatek){
    for (int i = 1; i < jatek.meret; i++) //1-tol mert az oszto nem kell
    {
        if(letezik(jatekostomb, jatek.jatekosok[i].nev)==-1){ //nem letezik
            Jatekos uj = {{*jatek.jatekosok[i].nev}, 0, 0}; //gcc miatt kell dupla {} (bug), nem teljesen ertem
            jatekostomb.meret++;
            jatekostomb.jatekosok = (Jatekos*)realloc(jatekostomb.jatekosok, jatekostomb.meret*sizeof(Jatekos));
            jatekostomb.jatekosok[jatekostomb.meret-1] = uj;
        }
    }
    return jatekostomb;
}

//egy uj jatek inditasaert felelos function es egyben almenu
//bemenet a jatekostomb amit vissza is ad, hogy menteni tudja
Jatekostomb ujjatek(Jatekostomb jatekostomb){
    econio_clrscr();
    Jatek jatek = {(Jatekban*)malloc(sizeof(Jatekban)), 1, 0, {0}}; //kezdetben eleg egy egy elemu tomb az osztonak, tobbi ertek: lasd fenti struct
    if(jatek.jatekosok == NULL){return jatekostomb;}
    strcpy(jatek.jatekosok[0].nev, "Oszto"); //oszto inicializalasa
    jatek.jatekosok[0].tet = 0;
    jatek.jatekosok[0].osszeg = 0;
    jatek.jatekosok[0].szek = -1;
    jatek.jatekosok[0].lapok[0] = '\0';

    char inp[10];

    asztal(jatek);
    menu();
    scanf("%s", inp); 
    if(!szame(inp)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont

    while(inp[0] != '9'){ //9 a vissza
        switch (inp[0])
        {
        case '0':
            jatek = leultetes(jatek, jatekostomb, 0); //jatekos leultetese
            jatekostomb = frissjatekosok(jatekostomb,jatek); //potencialis uj jatekosok mentese
            break;
        case '1':
            jatek = leultetes(jatek, jatekostomb, 1); //bot leultetese
            jatekostomb = frissjatekosok(jatekostomb,jatek);
            break;
        case '2':
            if(jatek.meret>1){
                jatek = felallitas(jatek);
                jatekostomb = frissjatekosok(jatekostomb,jatek);
            }
            break;
        case '3':
            printf("jatek\n");
            break;
        
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }
        asztal(jatek);
        menu();
        scanf("%s", inp); 
        if(!szame(inp)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
        
    }

    jatekostomb = frissjatekosok(jatekostomb,jatek); //torolheto? mar korabban mentve lettek

    free(jatek.jatekosok);
    return jatekostomb;
}