#include <stdio.h>

#include "econio.h"
#include "debugmalloc.h"
#include "jatek.h"
#include "jatekmenet.h"

//jatekos
//jatekos menu kiirasa
void jatekosmenu(){
    printf("Jatekosok szerkesztese - ");
    econio_textcolor(COL_BLUE);
    printf("0%s\n","\033[0m");

    printf("Jatekosok letrehozasa - ");
    econio_textcolor(COL_BLUE);
    printf("1%s\n","\033[0m");

    printf("Jatekos torlese - ");
    econio_textcolor(COL_BLUE);
    printf("2%s\n","\033[0m");

    printf("Vissza - ");
    econio_textcolor(COL_BLUE);
    printf("9%s\n","\033[0m");
}

//main
//menu kiirasa
void mainmenu(){
    printf("Uj jatek - ");
    econio_textcolor(COL_BLUE);
    printf("0%s\n","\033[0m");

    printf("Jatekosok - ");
    econio_textcolor(COL_BLUE);
    printf("1%s\n","\033[0m");

    printf("GYIK - ");
    econio_textcolor(COL_BLUE);
    printf("2%s\n","\033[0m");

    printf("Kilepes - ");
    econio_textcolor(COL_BLUE);
    printf("9%s\n","\033[0m");
}

//jatek
//menu kiirasa
void jatekmenu(){
    printf("Jatekos hozzadasa - ");
    econio_textcolor(COL_BLUE);
    printf("0%s\n","\033[0m");

    printf("Bot hozzadasa - ");
    econio_textcolor(COL_BLUE);
    printf("1%s\n","\033[0m");

    printf("Jatekos/Bot eltavolitasa - ");
    econio_textcolor(COL_BLUE);
    printf("2%s\n","\033[0m");

    printf("Jatek kezdese - ");
    econio_textcolor(COL_BLUE);
    printf("3%s\n","\033[0m");

    printf("Vissza - ");
    econio_textcolor(COL_BLUE);
    printf("9%s\n","\033[0m");
}

//jatek
//asztal rajzanak kiirasa (nevekkel)
//bemenet a jatek tomb
void asztalrajz(Jatek jatek){
    econio_clrscr();

    econio_gotoxy(48, 0); //elso sorban az asztal kozepere irjuk ki az osztot
    printf("%sOszto%s\n","\033[31m","\033[0m");
    econio_textbackground(COL_GREEN);
    econio_textcolor(COL_BLACK);
    printf("----------------------------------------------------------------------------------------------------\n");
    econio_textcolor(COL_BLUE);
    printf("%100s\r\n", "");//a sok uj sor az asztal helye (merete)
    printf("%100s\r\n", "");
    printf("%100s\r\n", "");
    printf("%100s\r\n", "");
    printf("%100s\r\n", "");
    econio_textcolor(COL_BLACK);
    printf("----------------------------------------------------------------------------------------------------\n");
    econio_textbackground(COL_BLACK);
    econio_textcolor(COL_LIGHTGRAY);

    for (int i = 1; i < jatek.meret; i++)
    {
        if(jatek.jatekosok[i].szek != -1){
            econio_gotoxy((jatek.jatekosok[i].szek)*20,8);
            printf("%s%s%s", "\033[34m",jatek.jatekosok[i].nev,"\033[0m");
        }
    }
    
    printf("\n\n\n\n"); //asztal alatti terulet
}

//jatekmenet
//a jatekos opcioit irja ki
//bemenet egy jatekban levo jatekos
void lapmenu(Asztalnal jatekos){
    econio_gotoxy(0,12);
    printf("Jatekos: ");
    econio_textcolor(COL_GREEN);
    printf("%s%s\n", jatekos.nev, "\033[0m");

    printf("Hit - ");
    econio_textcolor(COL_BLUE);
    printf("0%s\n","\033[0m");

    printf("Stand - ");
    econio_textcolor(COL_BLUE);
    printf("1%s\n","\033[0m");

    printf("Double - ");
    econio_textcolor(COL_BLUE);
    printf("2%s\n","\033[0m");

    printf("Surrender - ");
    econio_textcolor(COL_BLUE);
    printf("3%s\n","\033[0m");
}

//jatekmenet
//a botok lepesenel a nevuket irja ki
//bemenet egy jatekban levo jatekos
void botnev(Asztalnal jatekos){
    econio_gotoxy(0,12);
    printf("Jatekos: ");
    econio_textcolor(COL_GREEN);
    printf("%s%s\n", jatekos.nev, "\033[0m");
}

//kirajzolja a logot
void logo(){
    printf("\033[92m");
    printf("$$$$$$$\\  $$\\                     $$\\                               $$\\       \n");
    printf("$$  __$$\\ $$ |                    $$ |                              $$ |      \n");
    printf("$$ |  $$ |$$ | $$$$$$\\   $$$$$$$\\ $$ |  $$\\ $$\\  $$$$$$\\   $$$$$$$\\ $$ |  $$\\ \n");
    printf("$$$$$$$\\ |$$ | \\____$$\\ $$  _____|$$ | $$  |\\__| \\____$$\\ $$  _____|$$ | $$  |\n");
    printf("$$  __$$\\ $$ | $$$$$$$ |$$ /      $$$$$$  / $$\\  $$$$$$$ |$$ /      $$$$$$  / \n");
    printf("$$ |  $$ |$$ |$$  __$$ |$$ |      $$  _$$<  $$ |$$  __$$ |$$ |      $$  _$$<  \n");
    printf("$$$$$$$  |$$ |\\$$$$$$$ |\\$$$$$$$\\ $$ | \\$$\\ $$ |\\$$$$$$$ |\\$$$$$$$\\ $$ | \\$$\\ \n");
    printf("\\_______/ \\__| \\_______| \\_______|\\__|  \\__|$$ | \\_______| \\_______|\\__|  \\__|\n");
    printf("                                      $$\\   $$ |                              \n");
    printf("                                      \\$$$$$$  |                              \n");
    printf("                                       \\______/                               \n\n\n\n\n\n");
    printf("\033[0m");

    //forras: https://patorjk.com/software/taag/ (ascii art generator)
}