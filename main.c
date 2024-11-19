#include <stdio.h>
#include <stdlib.h>

#include "jatek.h"
#include "jatekos.h"
#include "gyik.h"
#include "main.h"
#include "econio.h"
#include "debugmalloc.h"
#include "konzol.h"

//megnezi, hogy az input egyjegyu szam-e
//bemenet az input, kimenet 0 vagy 1
int szame(char *inp, int menue){
    int hossz = strlen(inp);
    if(menue && hossz > 1){return 0;} //csak egyjegyu input van

    for (int i = 0; i < hossz; i++)
    {
        if(inp[i]-'0'<0 && '9'-inp[i]<0){ //ha a 0 es a 9 ascii kodja koze esik akkor szam, ha nem akkor hamisat ad
            return 0;
        }
    }
    return 1;
}

int input(){
    char inp[10];
    econio_textcolor(COL_BLUE);
    scanf("%10s", inp);
    econio_textcolor(COL_LIGHTGRAY);
    if(!szame(inp,1)){return 8;} //ha nem szam vagy hosszabb mint 1 akkor nem letezo menupont
    int inpszam = inp[0]-'0';
    char extra = 0;
    while((extra=getchar())!='\n' && extra!=EOF); //20. beolvasott karakter utan levo karakterek ott maradnanak a bemeneten
    return inpszam;
}

//a fomenu function
int main(){
    econio_set_title("Blackjack");
    //TESZTHEZ innentol
    FILE *file;
    file = fopen("jatekosok.txt", "w");
    fprintf(file, "4\n");
    fprintf(file, "A 0000 0\n");
    fprintf(file, "B 1111 1\n");
    fprintf(file, "C 2222 2\n");
    fprintf(file, "D 3333 3\n");
    fclose(file);
    //idaig

    int meret = filemeret();
    Jatekostomb jatekostomb = {NULL, meret};
    if(meret != 0){
        jatekostomb = beolvas(jatekostomb);
    }else{
        fileletrehoz();
    }

    logo();
    mainmenu();

    int inp = 8;
    inp = input();
    while(inp!=9){ //3 a kilepes
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
                printf("%sNincs ilyen menupont!%s","\033[1;31m", "\033[0m");
                econio_sleep(3);
                break;
        }
        econio_clrscr();
        logo();
        mainmenu();
        inp = input();
    }

    printf("\033[0m");
    felszabadit(jatekostomb.jatekosok);
    return 0;
}