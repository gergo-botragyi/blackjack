#include <stdio.h>
#include <time.h>

#include "jatek.h"
#include "jatekos.h"
#include "main.h"
#include "jatekmenet.h"
#include "econio.h"
#include "debugmalloc.h"
#include "konzol.h"

//kiirja a mentett neveket a kepernyore
//bemenet az eddig mentett jatekostomb (ami ugye a fajlban levo neveket ismeri)
void nevekkiir(Jatekostomb jatekostomb){
    Jatekos *mozgo = jatekostomb.jatekosok;
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        printf("Jatekos: %s\n", mozgo->nev);
        mozgo = mozgo->kov;
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
        if(szekek[i]!=0){foglalt++;}
    }
    return foglalt;
}

//hozzaad egy uj jatekost a jatek tombhoz
//bemenet a jatek tomb, a hely ahova leul a jatekos es a neve
//noveli a jatekosok tombjet es inicializalja a jatekost az utolso helyre
//visszaadja a jatek tombot
Jatek jatekhozad(Jatek jatek, int hely, char *nev, int bot){
    jatek.meret++;
    int meret = jatek.meret;
    jatek.jatekosok = (Jatekban*)realloc(jatek.jatekosok, meret*sizeof(Jatekban));

    strcpy(jatek.jatekosok[meret-1].nev, nev);
    jatek.jatekosok[meret-1].nyeremeny = 0;
    jatek.jatekosok[meret-1].szek = hely;
    jatek.szekek[hely] = meret-1; //szeknel tarolja a jatekos indexet kesobbre
    jatek.jatekosok[meret-1].bot = bot;

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
    if(bot != 0){
        sprintf(nev, "Bot%d", ++jatek.botok);
        letrehozott = 1;
    }else{
        strcpy(nev,jatekosnev(jatekostomb, nev));
        letrehozott = letezik(jatekostomb, nev)!=NULL; //megnezi, hogy korabban volt e mar mentve (fajlba irva) a jatekos neve
    }

    int hely = uresszek(jatek.szekek);
    int meret = jatek.meret;
    
    if(letrehozott && !voltleultetve(jatek, nev) && asztalnal(jatek.szekek) < 5){ //ha mar ismeri a nevet de nem volt leultetve es van hely az asztalnal
        return jatekhozad(jatek, hely, nev, bot);
    }

    if(letrehozott && !jatszik(jatek, nev) && asztalnal(jatek.szekek) < 5){ //ismeri a nevet, volt mar asztalnal de nem jatszik jelenleg es van hely
        for (int i = 1; i < meret; i++)
        {
            if(strcmp(jatek.jatekosok[i].nev, nev)==0){
                jatek.jatekosok[i].szek = hely;
                jatek.szekek[hely] = i; //szeknel tarolja a jatekos indexet kesobbre
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
            jatek = jatekhozad(jatek, hely, nev, bot);
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
        for (int i = 1; i < jatek.meret; i++)
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
        char *nev = jatek.jatekosok[i].nev;
        if(jatek.jatekosok[i].bot == 0 && letezik(jatekostomb, nev)==NULL){ //nem letezik es nem bot
            Jatekos *uj = (Jatekos*)malloc(sizeof(Jatekos));
            strcpy(uj->nev, nev);
            uj->korok = 0;
            uj->nyeremeny = 0;
            uj->kov = NULL;

            jatekostomb.meret++;
            jatekoshozzaad(jatekostomb.jatekosok, uj);
        }
    }
    return jatekostomb;
}

void adatmentes(Jatekostomb jatekostomb, Jatek jatek){
    Jatekos *mozgo = jatekostomb.jatekosok;
    while(mozgo!=NULL){
        for (int i = 1; i < jatek.meret; i++){
            if(jatek.jatekosok[i].bot == 0 && strcmp(mozgo->nev, jatek.jatekosok[i].nev) == 0){
                mozgo->korok++;
                mozgo->nyeremeny += jatek.jatekosok[i].nyeremeny;
            }
        }
        mozgo = mozgo->kov;
    }
}

Asztal asztalletrehoz(Jatek jatek, Asztal asztal){
    strcpy(asztal.jatekosok[0].nev, "Oszto");
    asztal.jatekosok[0].tet = 0;
    asztal.jatekosok[0].laposszeg = 0;
    asztal.jatekosok[0].lapszam = 0;
    asztal.jatekosok[0].szek = -1;
    asztal.jatekosok[0].bot = 17; //az oszto 17-nel kell megalljon
    asztal.jatekosok[0].vesztett = 0;

    int j = 1;
    for (int i = 1; i < jatek.meret; i++)
    {
        if(jatek.jatekosok[i].szek != -1){
            strcpy(asztal.jatekosok[j].nev, jatek.jatekosok[i].nev);
            asztal.jatekosok[j].tet = 0;
            asztal.jatekosok[j].lapszam = 0;
            asztal.jatekosok[j].laposszeg = 0;
            asztal.jatekosok[j].szek = jatek.jatekosok[i].szek;
            asztal.jatekosok[j].bot = jatek.jatekosok[i].bot;
            asztal.jatekosok[j].vesztett = 0;
            j++;
        }
    }
    asztal.meret = j;
    return asztal;
}

Jatek asztalment(Jatek jatek, Asztal asztal){    
    for (int i = 1; i < jatek.meret; i++)
    {
        int index = jatek.szekek[asztal.jatekosok[i].szek]; //a jatek.szek tarolja a jatekos indexet aki ott ul
        jatek.jatekosok[index].nyeremeny += asztal.jatekosok[i].tet; 
    }
    return jatek;
}

//egy uj jatek inditasaert felelos function es egyben almenu
//bemenet a jatekostomb amit vissza is ad, hogy menteni tudja
Jatekostomb ujjatek(Jatekostomb jatekostomb){
    srand(time(NULL));
    Asztal asztal = {(Asztalnal*)malloc(6*sizeof(Asztalnal)), 0};

    Jatek jatek = {(Jatekban*)malloc(sizeof(Jatekban)), 1, 0, {0}}; //kezdetben eleg egy egy elemu tomb az osztonak, tobbi ertek: lasd fenti struct
    if(jatek.jatekosok == NULL){return jatekostomb;}
    strcpy(jatek.jatekosok[0].nev, "Oszto"); //oszto inicializalasa
    jatek.jatekosok[0].nyeremeny = 0;
    jatek.jatekosok[0].szek = -1;
    jatek.jatekosok[0].bot = (rand()%(17 - 15 +1)+15);

    char inp[10];

    econio_clrscr();
    asztalrajz(jatek); //asztal megjelenítése a képernyőn
    jatekmenu(); //menü megjelenítése a képernyőn
    scanf("%s", inp); 
    if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont

    while(inp[0] != '9'){ //9 a vissza
        switch (inp[0])
        {
        case '0':
            jatek = leultetes(jatek, jatekostomb, 0); //jatekos leultetese
            jatekostomb = frissjatekosok(jatekostomb,jatek); //potencialis uj jatekosok mentese
            break;
        case '1':
            jatek = leultetes(jatek, jatekostomb, (rand()%(17 - 15 +1)+15)); //bot leultetese (random valtozo 15 es 17 kozott a bot jatekstilusa (mire nem ker mar lapot))
            jatekostomb = frissjatekosok(jatekostomb,jatek);
            break;
        case '2':
            if(jatek.meret>1){
                jatek = felallitas(jatek);
                jatekostomb = frissjatekosok(jatekostomb,jatek);
            }
            break;
        case '3':
            asztal = asztalletrehoz(jatek, asztal);
            asztal = jatekmenet(asztal);
            jatek = asztalment(jatek, asztal);
            adatmentes(jatekostomb, jatek);
            break;
        
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(3);
            break;
        }
        asztalrajz(jatek);
        jatekmenu();
        scanf("%s", inp); 
        if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
        
    }

    filebair(jatekostomb);
    free(asztal.jatekosok);
    free(jatek.jatekosok);
    return jatekostomb;
}