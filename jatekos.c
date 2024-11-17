#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jatekos.h"
#include "main.h"
#include "econio.h"
#include "debugmalloc.h"
#include "konzol.h"

//megmondja hany mentett jatekos van
//megnyitja a jatekosok.txt fajlt
//annak elso sorabol beolvassa, hogy hany mentett jatekost tartalmaz a fajl es azt visszaadja
int filemeret(){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){return 0;}

    int meret = 0;
    fscanf(file, "%d" , &meret);
    fclose(file);
    return meret;
}

//beolvassa a jatekosokat fajlbol
//bemenetkent egy jatekostombot kap egy ekkor ures jatekosokbol allo tombbel es egy merettel
//megnyitja a jatekosok.txt faljt
//az elso sor nem kell, mert a meretet mar tudjuk ekkor
//soronkent beolvas es letrehoz egy uj jatekost, majd hozzaadja a tombhoz
Jatekostomb beolvas(Jatekostomb jatekostomb){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}
    while(getc(file) != '\n'); //skip first line

    char nev[21];
    int nyeremeny, korok;
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        fscanf(file, "%s %d %d", nev, &nyeremeny, &korok);
        Jatekos *uj = (Jatekos*)malloc(sizeof(Jatekos));
        strcpy(uj->nev, nev);
        uj->nyeremeny = nyeremeny;
        uj->korok = korok;
        uj->kov = NULL;
        if(i==0){
            jatekostomb.jatekosok = uj;
        }else{
            jatekoshozzaad(jatekostomb.jatekosok, uj);
        }
    }
    return jatekostomb;
    fclose(file);
}

//kiirja a kepernyore a jatekosok adatait
//bemenetkent egy jatekostombot kap (jatekosok tomb es meret)
void kiir(Jatekostomb jatekostomb){
    Jatekos *mozgo = jatekostomb.jatekosok;
    while(mozgo!=NULL){
        printf("Nev: %s \nNyert osszeg: %d \nJatszott korok: %d\n\n", mozgo->nev, mozgo->nyeremeny, mozgo->korok);
        mozgo = mozgo->kov;

    }
}

//elmenti a jatekosokat fajlba
//bemenetkent egy jatekostombot kap (jatekosok tomb es meret)
//megnyitja a jatekosok.txt fajlt
//kiirja a fajlba eloszor a jatekosok szamat (meret) es utana minden sorba egy jatekos adatait
void filebair(Jatekostomb jatekostomb){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");return;}
    fprintf(file, "%d\n" ,jatekostomb.meret);

    Jatekos *mozgo = jatekostomb.jatekosok;
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        fprintf(file, "%s %d %d\n", mozgo->nev, mozgo->nyeremeny, mozgo->korok);
        mozgo = mozgo->kov;
    }
    fclose(file);
}

//letrehoz egy jatekosok.txt nevu fajlt es beleirja az elso sorba, hogy 0 jatekos van
void fileletrehoz(){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");}

    fprintf(file, "%d", 0);
    fclose(file);
}

void felszabadit(Jatekos *eleje){
    Jatekos *mozgo = eleje;
    while(mozgo !=NULL){
        Jatekos *kov = mozgo->kov;
        free(mozgo);
        mozgo = kov;
    }
}

//megnezi hogy letezik-e egy jatekos
//bemenetkent kap egy jatekostombot (jatekosokbol allo tomb es meret) valamint egy nevet
//vegigmeny a jatekosok tombon es megnezi, hogy a megadott nevvel letezik e olyan jatekos, ha igen visszaadja az indexet, ha nem -1-et
Jatekos* letezik(Jatekostomb jatekostomb, char *reginev){
    Jatekos *mozgo = jatekostomb.jatekosok;
    while(mozgo!=NULL){
        if(strcmp(mozgo->nev, reginev)==0){
            return mozgo;
        }
        mozgo = mozgo->kov;
    }
    return mozgo;
}

//jatekosok nevenek szerkesztese
//bementkent kap egy jatekostombot (jatekosokbol allo tomb es meret)
//kiirja a jatekosokat, beker egy szerkeszteni kivant jatekos nevet, es ha letezik atirja azt
//visszaadja a jatekostombot
Jatekostomb szerkesztes(Jatekostomb jatekostomb){
    econio_clrscr();
    kiir(jatekostomb);

    char reginev[21]; //max 20 karakter egy nev + lezaro nulla
    char ujnev[21];
    econio_clrscr();

    kiir(jatekostomb);
    printf("Szerkeszteni kivant jatekos neve: ");
    scanf("%s", reginev);

    Jatekos *keresett = letezik(jatekostomb, reginev);
    if(keresett == NULL){
        printf("Nincs ilyen nevu jatekos!");
        econio_sleep(3);
        return jatekostomb;
    }

    printf("Uj nev: ");
    scanf("%s", ujnev);

    strcpy(keresett->nev, ujnev);

    return jatekostomb;
}

void jatekoshozzaad(Jatekos *eleje, Jatekos *uj){
    Jatekos *mozgo = eleje;
    while(mozgo->kov!=NULL){
        mozgo = mozgo->kov;
    }

    mozgo->kov = uj;
}


//jatekosokat lehet letrehozni
//bemenetkent kap egy jatekostombot (jatekosokbol allo tomb es meret)
//bekeri az uj jatekos nevet es ha meg nincs ilyen nevu jatekos akkor letrehozza es hozzaadja a listahoz
//visszaadja a jatekostombot
Jatekostomb letrehozas(Jatekostomb jatekostomb){
    econio_clrscr();
    if(jatekostomb.meret == 0){printf("Meg nincsenek letrehozott jatekosok!\n");}

    econio_clrscr();
    kiir(jatekostomb);

    Jatekos *uj = (Jatekos*)malloc(sizeof(Jatekos));
    uj->nyeremeny = 0;
    uj->korok = 0;
    uj->kov = NULL;

    printf("Uj jatekos neve: ");
    scanf("%s", uj->nev);

    Jatekos *keresett = letezik(jatekostomb, uj->nev);
    if(keresett == NULL){
        jatekostomb.meret++;
        jatekoshozzaad(jatekostomb.jatekosok, uj);

        econio_clrscr();
        kiir(jatekostomb);
    }else{
        printf("Mar van ilyen nevu jatekos!\n");
        econio_sleep(3);
    }

    return jatekostomb;
}

Jatekostomb jatekostorol(Jatekostomb jatekostomb, char *nev){
    Jatekos *lemarado = NULL;
    Jatekos *mozgo = jatekostomb.jatekosok;
    while(mozgo!=NULL && strcmp(mozgo->nev, nev) != 0){
        lemarado = mozgo;
        mozgo = mozgo->kov;
    }
    if(mozgo==NULL){
        return jatekostomb;
    }else if(lemarado == NULL){
        Jatekos *ujeleje = mozgo->kov;
        free(mozgo);
        jatekostomb.jatekosok = ujeleje;
        jatekostomb.meret--;
    }else{
        jatekostomb.meret--;
        lemarado->kov = mozgo->kov;
        free(mozgo);
    }
    return jatekostomb;
}

Jatekostomb torles(Jatekostomb jatekostomb){
    econio_clrscr();
    kiir(jatekostomb);

    char nev[21];
    printf("Torlendo jatekos neve: ");
    scanf("%s", nev);

    Jatekos *keresett = letezik(jatekostomb, nev);
    if(keresett != NULL){
        jatekostomb = jatekostorol(jatekostomb, nev);
    }else{
        printf("Nem letezik ilyen nevu jatekos!\n");
        econio_sleep(3);
    }
    
    return jatekostomb;
}

//a fo jatekos menu 
//megkapja a main-tol a jatekostombot (jatekosokbol allo tomb es merete)
//valasztani lehet az kulonbozo menupontokbol
//addig var uj inputra amig ki nem akarunk lepni es azt a szamot nem kapja inputnak, ekkor visszaadja a main-nek a jatekostombot
Jatekostomb jatekos(Jatekostomb jatekostomb){
    econio_clrscr();

    kiir(jatekostomb);
    jatekosmenu();

    char inp[10];
    scanf("%s", inp); 
    if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont

    while(inp[0] != '9'){ //9 a vissza
        switch (inp[0])
        {
        case '0':        
            if(jatekostomb.meret != 0){
                szerkesztes(jatekostomb);
            }else{ //ha meg nem leteznek jatekosok nincs kit szerkeszteni
                jatekostomb = letrehozas(jatekostomb);
            }
            break;
        case '1':
            jatekostomb = letrehozas(jatekostomb);
            break;
        case '2':
            if(jatekostomb.meret != 0){
                jatekostomb = torles(jatekostomb);
            }
            break;
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(3);
            break;
        }

        econio_clrscr();
        kiir(jatekostomb);
        jatekosmenu();
        scanf("%s", inp); 
        if(!szame(inp,1)){inp[0] = '8';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
    }

    filebair(jatekostomb);
    return jatekostomb;
}