#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functions.h"
#include "econio.h"

typedef struct Jatekos{
    char nev[50];
    int nyeremeny;
    int korok;
} Jatekos;

int filemeret(){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){return 0;}

    int meret;
    fscanf(file, "%d" , &meret);
    return meret;
}

Jatekos *beolvas(int meret){
    FILE *file = fopen("jatekosok.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}
    while(getc(file) != '\n'); //skip first line

    Jatekos *adatok = calloc(meret,sizeof(Jatekos));
    if(!adatok){return NULL;}

    Jatekos uj;
    for (int i = 0; i < meret; i++)
    {
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

void filebair(Jatekos *jatekosok, int meret){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");}
    fprintf(file, "%d\n" ,meret);

    for (int i = 0; i < meret; i++)
    {
        fprintf(file, "%s %d %d\n", jatekosok[i].nev, jatekosok[i].nyeremeny, jatekosok[i].korok);
    }
    fclose(file);
}

void fileletrehoz(){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");}

    fprintf(file, "%d", 0);
    fclose(file);
}

int letezik(Jatekos *jatekosok, int meret, char reginev[50]){
    int i = 0;
    bool bennevan = false;
    while(!bennevan && i < meret){
        if(strcmp(jatekosok[i].nev, reginev) == 0){
            bennevan = true;
        }
        i++;
    }
    return bennevan ? i-1 : -1;
}

void szerkesztes(Jatekos *jatekosok, int meret){
    econio_clrscr();
    kiir(jatekosok, meret);

    int inp;
    printf("Szerkesztes - 0\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    char reginev[50];
    char ujnev[50];
    while(inp != 9){
        if(inp == 0){
            econio_clrscr();

            kiir(jatekosok, meret);
            printf("Szerkeszteni kivant jatekos neve: ");
            scanf("%s", reginev);

            printf("Uj nev: ");
            scanf("%s", ujnev);

            int index = letezik(jatekosok, meret, reginev);
            if(index != -1){
                strcpy(jatekosok[index].nev, ujnev);
            }else{
                printf("Nincs ilyen nevu jatekos!");
                econio_sleep(2);
            }

            econio_clrscr();

            kiir(jatekosok, meret);
            printf("Szerkesztes - 0\n");
            printf("Vissza - 9\n");
            scanf("%d", &inp);
        }else{
            printf("Nincs ilyen menupont! Adj meg egy ujat!\n");
            scanf("%d", &inp);
        }
    }
    filebair(jatekosok, meret);
}

int letrehozas(Jatekos *jatekosok, int meret){
    FILE *file = fopen("jatekosok.txt", "w");
    if(file == NULL){printf("Unable to open file!");}

    econio_clrscr();
    if(meret == 0){printf("Meg nincsenek letrehozott jatekosok!\n");}

    int inp;
    kiir(jatekosok, meret);
    printf("Letrehozas - 0\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    while(inp != 9){
        if(inp == 0){
            econio_clrscr();
            kiir(jatekosok, meret);

            Jatekos uj;
            uj.nyeremeny = 0;
            uj.korok = 0;

            printf("Uj jatekos neve: ");
            scanf("%s", uj.nev);

            int index = letezik(jatekosok, meret, uj.nev);
            if(index == -1){
                meret++;
                jatekosok = realloc(jatekosok, meret*sizeof(Jatekos));
                jatekosok[meret-1] = uj;

                econio_clrscr();
                kiir(jatekosok, meret);
                printf("Letrehozas - 0\n");
                printf("Vissza - 9\n");
                scanf("%d", &inp);
            }else{
                printf("Mar van ilyen nevu jatekos!\n");
                printf("Letrehozas - 0\n");
                printf("Vissza - 9\n");
                scanf("%d", &inp);
            }

        }else{
            printf("Nincs ilyen menupont! Adj meg egy ujat!\n");
            scanf("%d", &inp);
        }
    }

    filebair(jatekosok, meret);
    return meret;
}

int jatekosok(){
    econio_clrscr();

    FILE *file;
    file = fopen("jatekosok.txt", "w");
    fprintf(file, "4\n");
    fprintf(file, "A 0000 0\n");
    fprintf(file, "B 1111 1\n");
    fprintf(file, "C 2222 2\n");
    fprintf(file, "D 3333 3\n");
    fclose(file);

    int inp;
    printf("Jatekosok szerkesztese - 0\n");
    printf("Jatekosok letrehozasa - 1\n");
    printf("Vissza - 9\n");
    scanf("%d", &inp);

    int meret;
    Jatekos *jatekosok;
    if(inp != 9){
        meret = filemeret();
        if(meret != 0){
            jatekosok = beolvas(meret);
        }else{
            fileletrehoz();
        }
    }


    while(inp != 9){
        switch (inp)
        {
        case 0:        
            if(meret != 0){
                szerkesztes(jatekosok, meret);
            }else{
                meret = letrehozas(jatekosok, meret);
            }
            break;
        
        case 1:
            meret = letrehozas(jatekosok, meret);
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
    
    free(jatekosok);
    return 0;
}