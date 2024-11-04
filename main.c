#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

int szame(char *inp){
    for (int i = 0; i < strlen(inp); i++)
    {
        if(inp[i]-'0'<0 && '9'-inp[i]<0){ //ascii kodok
            return 0;
        }
    }
    return 1;
}

int main(){
    FILE *file;
    file = fopen("jatekosok.txt", "w");
    fprintf(file, "4\n");
    fprintf(file, "A 0000 0\n");
    fprintf(file, "B 1111 1\n");
    fprintf(file, "C 2222 2\n");
    fprintf(file, "D 3333 3\n");
    fclose(file);

    int meret = filemeret();
    Jatekostomb jatekostomb = {(Jatekos*)calloc(meret, sizeof(Jatekos)), meret};
    if(meret != 0){
        beolvas(jatekostomb);
    }else{
        fileletrehoz();
    }


    printf("Uj jatek - 0\n");
    printf("Jatekosok - 1\n");
    printf("GYIK - 2\n");
    printf("Kilepes - 3\n");

    char inp[10];
    scanf("%s", inp); 
    if(!szame(inp)){inp[0] = '9';} //input vagy nem letezo menupont
    while(inp[0]!='3'){
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