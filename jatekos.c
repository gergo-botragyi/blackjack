#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

int filemeret(){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){return 0;}

    int meret;
    fscanf(file, "%d" , &meret);
    fclose(file);
    return meret;
}

void beolvas(Jatekostomb jatekostomb){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}
    while(getc(file) != '\n'); //skip first line

    Jatekos uj;
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        fscanf(file, "%s %d %d", uj.nev, &uj.nyeremeny, &uj.korok);
        jatekostomb.jatekosok[i] = uj;
    }
    fclose(file);
}

/*void kiirFile(){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}

    char nev[50];
    int nyeremeny, korok;
    while(fscanf(file, "%s %d %d", nev, &nyeremeny, &korok) != EOF){
        printf("Nev: %s\n", nev);
        printf("Nyert osszeg: %d\n", nyeremeny);
        printf("Jatszott korok: %d\n", korok);
        printf("\n");
    }
    fclose(file);
}*/

void kiir(Jatekostomb jatekostomb){
    for (int i = 0; i < jatekostomb.meret; i++)
    {
        printf("Nev: %s \nNyert osszeg: %d \nJatszott korok: %d\n\n", jatekostomb.jatekosok[i].nev, jatekostomb.jatekosok[i].nyeremeny, jatekostomb.jatekosok[i].korok);
    }
}

void filebair(Jatekostomb jatekostomb){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");return;}
    fprintf(file, "%d\n" ,jatekostomb.meret);

    for (int i = 0; i < jatekostomb.meret; i++)
    {
        fprintf(file, "%s %d %d\n", jatekostomb.jatekosok[i].nev, jatekostomb.jatekosok[i].nyeremeny, jatekostomb.jatekosok[i].korok);
    }
    fclose(file);
}

void fileletrehoz(){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");}

    fprintf(file, "%d", 0);
    fclose(file);
}

int letezik(Jatekostomb jatekostomb, char *reginev){
    int i = 0;
    bool bennevan = false;
    while(!bennevan && i < jatekostomb.meret){
        if(strcmp(jatekostomb.jatekosok[i].nev, reginev) == 0){
            bennevan = true;
        }
        i++;
    }
    return bennevan ? i-1 : -1;
}

Jatekostomb szerkesztes(Jatekostomb jatekostomb){
    econio_clrscr();
    kiir(jatekostomb);

    int inp;
    printf("Szerkesztes - 0\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    char reginev[50];
    char ujnev[50];
    while(inp != 9){
        if(inp == 0){
            econio_clrscr();

            kiir(jatekostomb);
            printf("Szerkeszteni kivant jatekos neve: ");
            scanf("%s", reginev);

            printf("Uj nev: ");
            scanf("%s", ujnev);

            int index = letezik(jatekostomb, reginev);
            if(index != -1){
                strcpy(jatekostomb.jatekosok[index].nev, ujnev);
            }else{
                printf("Nincs ilyen nevu jatekos!");
                econio_sleep(2);
            }

            econio_clrscr();

            kiir(jatekostomb);
            printf("Szerkesztes - 0\n");
            printf("Vissza - 9\n");
            scanf("%d", &inp);
        }else{
            printf("Nincs ilyen menupont! Adj meg egy ujat!\n");
            scanf("%d", &inp);
        }
    }
    filebair(jatekostomb);
    return jatekostomb;
}

Jatekos *ujjatekos(Jatekos *jatekosok, int meret, Jatekos uj){
    Jatekos *seged = (Jatekos*) calloc(meret,sizeof(Jatekos));
    for (int i = 0; i < meret-1; i++)
    {
        strcpy(seged[i].nev, jatekosok[i].nev);
        seged[i].nyeremeny = jatekosok[i].nyeremeny;
        seged[i].korok = jatekosok[i].korok;
    }
    strcpy(seged[meret-1].nev, uj.nev);
    seged[meret-1].nyeremeny = 0;
    seged[meret-1].korok = 0;
                
    free(jatekosok);
    jatekosok = (Jatekos*) calloc(meret,sizeof(Jatekos));
                
    for (int i = 0; i < meret; i++)
    {
        strcpy(jatekosok[i].nev, seged[i].nev);
        jatekosok[i].nyeremeny = seged[i].nyeremeny;
        jatekosok[i].korok = seged[i].korok;
    }
    free(seged);
    return jatekosok;
}

Jatekostomb letrehozas(Jatekostomb jatekostomb){
    econio_clrscr();
    if(jatekostomb.meret == 0){printf("Meg nincsenek letrehozott jatekosok!\n");}

    int inp;
    kiir(jatekostomb);
    printf("Letrehozas - 0\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    while(inp != 9){
        if(inp == 0){
            econio_clrscr();
            kiir(jatekostomb);

            Jatekos uj;
            uj.nyeremeny = 0;
            uj.korok = 0;

            printf("Uj jatekos neve: ");
            scanf("%s", uj.nev);

            int index = letezik(jatekostomb, uj.nev);
            if(index == -1){
                jatekostomb.meret++;
                //jatekosok = ujjatekos(jatekosok, uj);
                jatekostomb.jatekosok = (Jatekos*)realloc(jatekostomb.jatekosok, jatekostomb.meret*sizeof(Jatekos));
                jatekostomb.jatekosok[jatekostomb.meret-1] = uj;

                /*strcpy(jatekostomb.jatekosok[jatekostomb.meret-1].nev, uj.nev);
                jatekostomb.jatekosok[jatekostomb.meret-1].nyeremeny = 0;
                jatekostomb.jatekosok[jatekostomb.meret-1].korok = 0;*/

                econio_clrscr();
                kiir(jatekostomb);
            }else{
                printf("Mar van ilyen nevu jatekos!\n");
            }
            printf("Letrehozas - 0\n");
            printf("Vissza - 9\n");
            scanf("%d", &inp);

        }else{
            printf("Nincs ilyen menupont! Adj meg egy ujat!\n");
            scanf("%d", &inp);
        }
    }

    filebair(jatekostomb);
    return jatekostomb;
}

Jatekostomb jatekos(Jatekostomb jatekosok){
    econio_clrscr();

    int inp;
    printf("Jatekosok szerkesztese - 0\n");
    printf("Jatekosok letrehozasa - 1\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    while(inp != 9){
        switch (inp)
        {
        case 0:        
            if(jatekosok.meret != 0){
                szerkesztes(jatekosok);
            }else{
                jatekosok = letrehozas(jatekosok);
            }
            break;
        
        case 1:
            jatekosok = letrehozas(jatekosok);
            break;
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }

        econio_clrscr();
        printf("Jatekosok szerkesztese - 0\n");
        printf("Jatekosok letrehozasa - 1\n");
        printf("Vissza - 9\n");
        scanf("%d", &inp);
    }

    return jatekosok;
}