#include <stdio.h>
#include "functions.h"
#include "econio.h"
#include "debugmalloc.h"

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

    int inp;
    scanf("%d", &inp);
    while(inp!=3){
        switch (inp)
        {
            case 0:
                jatekostomb = ujjatek(jatekostomb);
                break;
            case 1:
                jatekostomb = jatekos(jatekostomb);
                break;
            case 2:
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
        scanf("%d", &inp);
    }

    free(jatekostomb.jatekosok);
    return 0;
}