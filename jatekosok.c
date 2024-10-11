#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "econio.h"

typedef struct Jatekos{
    char nev[50];
    int nyeremeny;
    int korok;
} Jatekos;

Jatekos *olvas(int meret){
    Jatekos *adatok = calloc(meret,sizeof(Jatekos));
    if(!adatok){return NULL;}

    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}

    for (int i = 0; i < meret; i++)
    {
        Jatekos uj;
        fscanf(file, "%s %d %d", uj.nev, &uj.nyeremeny, &uj.korok);
        adatok[i] = uj;
    }
    fclose(file);

    return adatok;
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

void kiir(Jatekos *jatekosok, int meret){
    for (int i = 0; i < meret; i++)
    {
        printf("Nev: %s \nNyert osszeg: %d \nJatszott korok: %d\n\n", jatekosok[i].nev, jatekosok[i].nyeremeny, jatekosok[i].korok);
    }
}

void ir(Jatekos *jatekosok, int meret){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file! Check if file is missing!");}

    for (int i = 0; i < meret; i++)
    {
        fprintf(file, "%s %d %d\n", jatekosok[i].nev, jatekosok[i].nyeremeny, jatekosok[i].korok);
    }
    fclose(file);
}

int jatekosok(){
    econio_clrscr();

    FILE *fptr;
    fptr = fopen("jatekosok.txt", "w");
    fprintf(fptr, "A 0000 0\n");
    fprintf(fptr, "B 1111 1\n");
    fprintf(fptr, "C 2222 2\n");
    fprintf(fptr, "D 3333 3\n");
    fclose(fptr);

    Jatekos *jatekosok = olvas(4);
    kiir(jatekosok, 4);

    printf("Szerkesztes - 0\n");
    printf("Vissza - 1\n");
    int inp;
    scanf("%d", &inp);

    char reginev[50];
    char ujnev[50];
    while(inp == 0){
        econio_clrscr();

        printf("Szerkeszteni kivant jatekos neve: ");
        scanf("%s", reginev);

        printf("Uj nev: ");
        scanf("%s", ujnev);

        for (int i = 0; i < 4; i++)
        {
            if(strcmp(jatekosok[i].nev, reginev) == 0){
                strcpy(jatekosok[i].nev, ujnev);
            }
        }
        
        econio_clrscr();

        kiir(jatekosok, 4);
        printf("Szerkesztes - 0\n");
        printf("Vissza - 1\n");
        scanf("%d", &inp);
    }
    ir(jatekosok, 4);
    
    free(jatekosok);
    return 0;
}