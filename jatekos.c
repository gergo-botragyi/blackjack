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

    char reginev[50];
    char ujnev[50];
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

    filebair(jatekostomb);
    return jatekostomb;
}

Jatekostomb letrehozas(Jatekostomb jatekostomb){
    econio_clrscr();
    if(jatekostomb.meret == 0){printf("Meg nincsenek letrehozott jatekosok!\n");}

    kiir(jatekostomb);

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
        jatekostomb.jatekosok = (Jatekos*)realloc(jatekostomb.jatekosok, jatekostomb.meret*sizeof(Jatekos));
        jatekostomb.jatekosok[jatekostomb.meret-1] = uj;

        econio_clrscr();
        kiir(jatekostomb);
    }else{
        printf("Mar van ilyen nevu jatekos!\n");
    }

    filebair(jatekostomb);
    return jatekostomb;
}

Jatekostomb jatekos(Jatekostomb jatekostomb){
    econio_clrscr();

    kiir(jatekostomb);
    printf("Jatekosok szerkesztese - 0\n");
    printf("Jatekosok letrehozasa - 1\n");
    printf("Vissza - 9\n");

    char inp[10];
    scanf("%s", inp); 
    if(!szame(inp)){inp[0] = 9;} //input vagy nem letezo menupont

    while(inp[0] != '9'){
        switch (inp[0])
        {
        case '0':        
            if(jatekostomb.meret != 0){
                szerkesztes(jatekostomb);
            }else{
                jatekostomb = letrehozas(jatekostomb);
            }
            break;
        
        case '1':
            jatekostomb = letrehozas(jatekostomb);
            break;
        default:
            printf("Nincs ilyen menupont!");
            econio_sleep(2);
            break;
        }

        econio_clrscr();
        kiir(jatekostomb);
        printf("Jatekosok szerkesztese - 0\n");
        printf("Jatekosok letrehozasa - 1\n");
        printf("Vissza - 9\n");
        scanf("%s", inp); 
        if(!szame(inp)){inp[0] = '9';} //input vagy nem letezo menupont
    }

    return jatekostomb;
}