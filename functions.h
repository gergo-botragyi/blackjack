int gyik();

typedef struct Jatekos{
    char nev[20];
    int nyeremeny;
    int korok;
} Jatekos;

typedef struct Jatekostomb{
    Jatekos *jatekosok;
    int meret;
} Jatekostomb;

Jatekostomb jatekos(Jatekostomb jatekostomb);
Jatekostomb ujjatek(Jatekostomb jatekostomb);

void kiir(Jatekostomb jatekostomb);
void filebair(Jatekostomb jatekostomb);
void fileletrehoz();
int letezik(Jatekostomb jatekostomb, char *reginev);
Jatekostomb szerkesztes(Jatekostomb jatekostomb);
Jatekostomb letrehozas(Jatekostomb jatekostomb);

void nevekkiir(Jatekostomb jatekosok);
void removetext(int kezdsor, int vegsor);

int filemeret();
void beolvas(Jatekostomb jatekostomb);

int szame(char *inp);