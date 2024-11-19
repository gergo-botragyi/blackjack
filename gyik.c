#include <stdio.h>
#include "gyik.h"
#include "econio.h"
#include "debugmalloc.h"

//a gyik menuert felelos function
int gyik(){
    econio_clrscr();
    FILE *file = fopen("gyik.txt", "r");
    if(file == NULL){printf("Unable to read file! Check if file is missing!");}

    char sor[256];
    fgets(sor, sizeof(sor), file);
    printf("%s%s%s","\033[36m",sor,"\033[0m");
    while(fgets(sor, sizeof(sor), file)){
        if(sor[0]=='-'){
            printf("\033[33m");
            printf("%s", sor);
            printf("\033[0m");
        }else{
            printf("%s", sor);
        }
    }
    printf("\n\n\n");
    printf("%sNyomj entert a tovabblepeshez!%s","\033[33m","\033[0m");
    getchar();

    return 0;
}