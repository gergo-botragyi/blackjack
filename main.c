#include <stdio.h>
#include "functions.h"
#include "econio.h"

int main(){
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
                ujjatek();
                break;
            case 1:
                jatekosok();
                break;
            case 2:
                gyik();
                break;
        }

        printf("Uj jatek - 0\n");
        printf("Jatekosok - 1\n");
        printf("GYIK - 2\n");
        printf("Kilepes - 3\n");
        scanf("%d", &inp);
    }
}