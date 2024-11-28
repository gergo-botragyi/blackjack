#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "jatek.h"
#include "jatekos.h"
#include "main.h"
#include "jatekmenet.h"
#include "econio.h"
#include "debugmalloc.h"
#include "konzol.h"

//kiirja a jatekosok tetjeit
//bemenet az asztal tomb, ez alapjan kiirja a megadott teteket
void tetkiir(Asztalnal jatekos){
    econio_gotoxy((jatekos.szek)*20,9);
    printf("%s%d%s","\033[0;33m", jatekos.tet,"\033[0m");
}

//kiirja a jatekosok laposszeget es lapjait
//bemenet egy jatekban levo jatekos, es egy "bool", hogy osztot ir-e ki
void lapotkiir(Asztalnal jatekos, int oszto){
    if(oszto){econio_gotoxy(48,3);}else{econio_gotoxy(jatekos.szek*20,5);}
    econio_textbackground(COL_GREEN);
    printf("%s[%d]%s", "\033[1;93m", jatekos.laposszeg,"\033[0m");
    
    if(oszto){econio_gotoxy(48,2);}else{econio_gotoxy(jatekos.szek*20,6);}
    for (int i = 0; i < jatekos.lapszam; i++)
    {
        if(jatekos.lapok[i].szin){
            printf("\033[42m\033[1;96m"); //cyan
        }else{printf("\033[42m\033[1;91m");} //red
        printf("%s ", jatekos.lapok[i].lap);
    }
    econio_textcolor(COL_LIGHTGRAY);
    econio_textbackground(COL_BLACK);
}

//kiirja ha a jatekos vesztett
//bemenet egy jatekban levo jatekos
void vesztett(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sVesztett!%s","\033[31m","\033[0m");
}

//kiirja ha a jatekos nyert
//bemenet egy jatekban levo jatekos
void nyert(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sNyert!%s","\033[32m","\033[0m");
}

//kiirja ha a dontetlen az eredmeny
//bemenet egy jatekban levo jatekos
void dontetlen(Asztalnal jatekos){
    econio_gotoxy(jatekos.szek*20, 10);
    printf("%sDontetlen!%s","\033[34m","\033[0m");
}

//a tetek megteteleert felelos function
//bemenet a jatekban levo jatekosok tombje (asztal tomb)
//kimenet a megvaltoztatott asztal tomb
Asztal tetek(Asztal asztal){
    for (int i = 1; i < asztal.meret; i++)
    {
        removetext(12,18);
        econio_gotoxy(0,12);
        if(asztal.jatekosok[i].bot != 0){ //ha bot akkor random tetje van
            econio_sleep(1);
            //rand() % (max - min + 1) + min  //a minimum maximum ertekek miatt
            asztal.jatekosok[i].tet = rand() % (5000 - 500 + 1) + 500;
            tetkiir(asztal.jatekosok[i]);
        }
        else{
            char tet[21];
            do
            {
                printf("Jatekos: %s%s%s\n","\033[0;32m", asztal.jatekosok[i].nev,"\033[0m");
                printf("Tet: ");
                printf("\033[33m"); //sarga
                scanf("%20s", tet);
                econio_textcolor(COL_LIGHTGRAY);
                char extra = 0;
                while((extra=getchar())!='\n' && extra!=EOF); //20. beolvasott karakter utan levo karakterek ott maradnanak a bemeneten
            } while (!szame(tet, 0)); //ha nem szamot adott meg ujra kerjuk
            asztal.jatekosok[i].tet = atoi(tet); //string atalakitasa int-te
            tetkiir(asztal.jatekosok[i]);
        }
    }
    return asztal;
}

//lapot ad egy jatekosnak
//bemenet egy jatekban levo jatekos es a lapok tomb
//kimenet a modositott jatekos
Asztalnal lapotkap(Asztalnal jatekos, Kartya *lapok){
    
    int index = (rand()%(12-0+1)+0); //random index
    int lapertek = lapok[index].ertek;
    //ha a jatekos laposszege + az uj lap erteke nagyobb lenne mint 21, akkor meg kell nezni, hogy van-e Asza
    //ha van nala Asz, meg kell nezni, hogy ha azt 1-nek szamoljuk ugy 21-en belul marad-e
    if(lapertek+jatekos.laposszeg>21){
        int i = 0;
        while(i<jatekos.lapszam && jatekos.laposszeg+lapertek>21){
            if(jatekos.lapok[i].ertek==11 && jatekos.laposszeg-10+lapertek<=21){
                jatekos.lapok[i].ertek = 1;
                jatekos.laposszeg -= 10;
            }
            i++;
        }
    }

    jatekos.lapok[jatekos.lapszam] = lapok[index]; //megkapja a lapot
    jatekos.lapok[jatekos.lapszam].szin = (rand()%(1-0+1)+0); //a lap szine random 0 vagy 1, ami kiirasnal cyan vagy piros
    jatekos.laposszeg+=lapertek; 
    if(lapertek==11 && jatekos.laposszeg>21){ //ha Aszt kapott, meg kell nezni, hogy 11-gyel szamolva kiment-e 21-bol, ha igen 1-nek veszi
        jatekos.laposszeg -= 10;
        jatekos.lapok[jatekos.lapszam].ertek = 1; 
    }
    jatekos.lapszam++;

    return jatekos;
}

//a kezdeti osztasert felelos function
//bemenet az asztal tomb, a lapok tomb, es hogy az oszto lapja le van-e forditva (kezdetben 0)
//kimenet a megvaltoztatott asztal tomb
Asztal osztas(Asztal asztal, Kartya *lapok, int leforditott){
    for (int i = 1; i < asztal.meret; i++) //eloszor minden jatekos kap lapot
    {
        asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
        lapotkiir(asztal.jatekosok[i], 0);
        econio_sleep(1);
    }
    asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok); //oszto kap lapot

    if(!leforditott){ //ha meg nem a leforditott (masodik) lapjat kapja az oszto, akkor rekurzivan meghivjuk megegyszer, mert 2-2 karytat kell osztani
        lapotkiir(asztal.jatekosok[0],1);
        econio_sleep(1);
        asztal = osztas(asztal, lapok, 1);
    }
    return asztal;
}

//a jatekmenet fo function-je
//bemenet es kimenet is az asztaltomb, amit bent modositunk persze
Asztal jatekmenet(Asztal asztal){
    srand(time(NULL)); //random miatt

    Kartya lapok[] = {
        {.lap = "2",.ertek=2},{.lap="3",.ertek=3},
        {.lap = "4",.ertek=4},{.lap="5",.ertek=5},
        {.lap = "6",.ertek=6},{.lap="7",.ertek=7},
        {.lap = "8",.ertek=8},{.lap="9",.ertek=9},
        {.lap = "10",.ertek=10},{.lap="J",.ertek=10},
        {.lap = "Q",.ertek=10},{.lap="K",.ertek=10},
        {.lap = "A",.ertek=11}
    };

    asztal = tetek(asztal); //tetek megtetele
    asztal = osztas(asztal, lapok, 0); //elso ket lap kiosztasa

    for (int i = 1; i < asztal.meret; i++)
    {
        int inp = 8;
        int botlep = asztal.jatekosok[i].bot > asztal.jatekosok[i].laposszeg; //igaz hamis, hogy lephet-e egy bot

        if(botlep){inp = 0; botnev(asztal.jatekosok[i]); econio_sleep(1);} //ha lephet a bot akkor 0 az input
        else if(asztal.jatekosok[i].bot > 0 && !botlep){inp = 1; botnev(asztal.jatekosok[i]); econio_sleep(1);} //amugy 1
        
        if(asztal.jatekosok[i].bot == 0 && asztal.jatekosok[i].laposszeg < 21){ //csak ha nem bot es meg kerhet lapot
            removetext(12,16);
            lapmenu(asztal.jatekosok[i]);
            inp = input();
        }

        while(inp!=1 && asztal.jatekosok[i].laposszeg<21){
            switch (inp)
            {
            case 0: //lapkeres
                asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
                lapotkiir(asztal.jatekosok[i], 0);
                if(asztal.jatekosok[i].laposszeg>21){
                    asztal.jatekosok[i].vesztett = 1;
                    asztal.jatekosok[i].tet *= -1; //tetjet elvesziti tehat nyeremenyehez a -1-szereset kell hozzaadni
                    vesztett(asztal.jatekosok[i]);
                    inp = 1;
                }
                if(asztal.jatekosok[i].bot > 0){econio_sleep(1);}
                break;
            case 2: //double
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i] = lapotkap(asztal.jatekosok[i], lapok);
                    lapotkiir(asztal.jatekosok[i], 0);
                    if(asztal.jatekosok[i].laposszeg>21){
                        asztal.jatekosok[i].vesztett = 1;
                        asztal.jatekosok[i].tet *= -1; //tetjet elvesziti tehat nyeremenyehez a -1-szereset kell hozzaadni
                        vesztett(asztal.jatekosok[i]);
                    }else{
                        asztal.jatekosok[i].tet *= 2; //tet duplazasa
                        tetkiir(asztal.jatekosok[i]);
                    }
                    inp = 1;
                }
                break;
            case 3: //surrender
                if(asztal.jatekosok[i].lapszam == 2){
                    asztal.jatekosok[i].tet *= -0.5; //csak a felet veszti el
                    tetkiir(asztal.jatekosok[i]);
                    asztal.jatekosok[i].vesztett = 1;
                    vesztett(asztal.jatekosok[i]);
                    inp = 1;
                }
                break;
            default:
                printf("%sNincs ilyen menupont!%s","\033[31m","\033[0m");
                econio_sleep(3);
                break;
            }
            botlep = asztal.jatekosok[i].bot > asztal.jatekosok[i].laposszeg;

            removetext(17,18);
            lapmenu(asztal.jatekosok[i]);

            if(inp != 1 && asztal.jatekosok[i].laposszeg<21 &&asztal.jatekosok[i].bot==0){ //jatekosnal ujra menu, amugy a botot leptetjuk
                inp = input();
            }else if(botlep && asztal.jatekosok[i].bot>0){
                inp = 0;
            }else{
                inp = 1;
            }
        }
        removetext(12,18);
    }
    removetext(12,18);

    lapotkiir(asztal.jatekosok[0],1); //oszto leforditott lapjanak megjelenitese
    econio_sleep(1);
    while(asztal.jatekosok[0].bot>asztal.jatekosok[0].laposszeg && asztal.jatekosok[0].laposszeg<21){ //oszto huzasa
        asztal.jatekosok[0] = lapotkap(asztal.jatekosok[0], lapok);
        lapotkiir(asztal.jatekosok[0], 1);
        econio_sleep(1);
    }

    int osztolaposszeg = asztal.jatekosok[0].laposszeg;
    int jatekoslaposszeg;
    for (int i = 1; i < asztal.meret; i++)
    {
        jatekoslaposszeg = asztal.jatekosok[i].laposszeg;
        if(!asztal.jatekosok[i].vesztett){
            if(jatekoslaposszeg == 21 && osztolaposszeg != 21){ //blackjacknel 3/2 szerese a tet
                asztal.jatekosok[i].tet /= 2;
                asztal.jatekosok[i].tet *= 3;
                nyert(asztal.jatekosok[i]);
            }else if(osztolaposszeg>21 || jatekoslaposszeg > osztolaposszeg){
                asztal.jatekosok[i].tet *= 2; //nyeresnel ketszerezodik a tet
                nyert(asztal.jatekosok[i]);
            }else if(osztolaposszeg>jatekoslaposszeg){
                asztal.jatekosok[i].tet *= -1;
                vesztett(asztal.jatekosok[i]);
            }else{
                asztal.jatekosok[i].tet = 0; //dontetlennel nem nyer semmit mert kvazi "visszakapja" a tetet
                dontetlen(asztal.jatekosok[i]);
            }
        }
    }
    
    econio_gotoxy(0,18);
    printf("%sNyomj entert a tovabblepeshez!%s","\033[33m","\033[0m");
    getchar();
    return asztal;
}