#include <stdio.h>
#include <stdlib.h>

#include "jatek.h"
#include "jatekos.h"
#include "gyik.h"
#include "main.h"
#include "econio.h"
#include "debugmalloc.h"

//megnezi, hogy az input egyjegyu szam-e
//bemenet az input, kimenet 0 vagy 1
int szame(char *inp){
    int hossz = strlen(inp);
    if(hossz > 1){return 0;} //csak egyjegyu input van

    for (int i = 0; i < hossz; i++)
    {
        if(inp[i]-'0'<0 && '9'-inp[i]<0){ //ha a 0 es a 9 ascii kodja koze esik akkor szam, ha nem akkor hamisat ad
            return 0;
        }
    }
    return 1;
}

//a fomenu function
int main(){
    FILE *file;
    file = fopen("jatekosok.txt", "w");
    fprintf(file, "4\n"); //TESZTELESHEZ CSAK
    fprintf(file, "A 0000 0\n");
    fprintf(file, "B 1111 1\n");
    fprintf(file, "C 2222 2\n");
    fprintf(file, "D 3333 3\n");
    fclose(file);

    int meret = filemeret();
    Jatekostomb jatekostomb = {(Jatekos*)calloc(meret, sizeof(Jatekos)), meret};
    if(jatekostomb.jatekosok == NULL){return 0;}
    if(meret != 0){
        jatekostomb = beolvas(jatekostomb);
    }else{
        fileletrehoz();
    }


    printf("Uj jatek - 0\n");
    printf("Jatekosok - 1\n");
    printf("GYIK - 2\n");
    printf("Kilepes - 3\n");

    char inp[10];
    scanf("%s", inp); 
    if(!szame(inp)){inp[0] = '9';} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
    while(inp[0]!='3'){ //3 a kilepes
        switch (inp[0])
        {
            case '0':
                jatekostomb = ujjatek(jatekostomb);
                break;
            case '1':
                jatekostomb = jatekos(jatekostomb);
                break;
            case '2':
                gyik();
                break;
            default:
                printf("Nincs ilyen menupont!");
                econio_sleep(2);
                break;
        }
        econio_clrscr();
        printf("Uj jatek - 0\n");
        printf("Jatekosok - 1\n");
        printf("GYIK - 2\n");
        printf("Kilepes - 3\n");
        scanf("%s", inp);
        if(!szame(inp)){inp[0] = '9';} //input vagy nem letezo menupont
    }

    free(jatekostomb.jatekosok);
    return 0;
}