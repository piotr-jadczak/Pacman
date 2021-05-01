#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<string.h>

#define WYS 21
#define SZER 19
#define ILOSC_MONET 176

#define RED   "\x1B[31m"
#define BLU   "\x1B[34m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

typedef enum {W_GORE, W_PRAWO, W_DOL, W_LEWO} t_kierunek;

typedef struct {
  int poz_x;
  int poz_y;
  t_kierunek ruch;
} t_duch;

typedef struct {
  int poz_x;
  int poz_y;
  t_kierunek ruch;
  int monety;
} t_gracz;


//**********Wyswietlanie elementow*************
void pokaz_plansze(int t[][SZER], int m[][SZER]);
void pokaz_menu(int wyn, char nick[]);

//**********Sterowanie duchem******************
void wstaw_ducha(t_duch *d, int t[][SZER]);
void przesun_ducha(t_duch *d);
void cofnij_ducha(t_duch *d);
bool czy_sciana(t_duch *d, int t[][SZER]);
bool czy_gracz(t_duch *d, int t[][SZER]);
void zmien_kierunek_d(t_duch *d, int t[][SZER]);
void usun_poprzedniego_d(t_duch *d, int t[][SZER]);
void steruj_duchem(t_duch *d, int t[][SZER], int *zycie);
//**********Sterowanie Pacmanem****************
void wstaw_pacmana(t_gracz *g, int t[][SZER]);
bool czy_moneta(t_gracz *g, int m[][SZER]);
bool czy_duch(t_gracz *g, int t[][SZER]);
void przesun_pacmana(t_gracz *g, int t[][SZER]);
void zmien_kierunek_p(t_gracz *g, int t[][SZER], char c);
char getch();
void steruj_pacmanem(t_gracz *g, int t[][SZER], int m[][SZER], int *zycie);
//***********Inicializacja planszy(wstawienie duchow i gracza), wczytanie najlepszego wyniku
void rozpocznij_gre(t_duch *d1, t_duch *d2, t_duch *d3, t_duch *d4, t_gracz *g, int t[][SZER], int *wyn, char nick[]);
//***********Wczytywanie i zapis wyniku
void wczytaj_wynik(int *wyn, char nick[]); //wczytuje najlepszy wynik, jesli nie ma pliku wynik = 0
void zapisz_wynik(int wyn, char nick[]);
void zapisz_lepszy_wynik(int wyn, int obecnyw);

int main () {
  srand( time( NULL ) );
  int plansza[WYS][SZER] = {
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
      {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1},
      {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
      {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1},
      {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
      {1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,1},
      {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1},
      {1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
      {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
      {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
      {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
      {1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1},
      {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
      {1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
  int monety[WYS][SZER] = {
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,0},
      {0,2,0,0,2,0,0,0,2,0,2,0,0,0,2,0,0,2,0},
      {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
      {0,2,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,2,0},
      {0,2,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,2,0},
      {0,0,0,0,2,0,0,0,2,0,2,0,0,0,2,0,0,0,0},
      {0,2,2,2,2,0,0,0,0,0,0,0,0,0,2,2,2,2,0},
      {0,2,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,2,0},
      {0,2,2,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,0},
      {0,2,0,0,2,0,0,0,0,0,0,0,0,0,2,0,2,0,0},
      {0,2,2,2,2,0,0,0,0,0,0,0,0,0,2,0,2,0,0},
      {0,0,0,0,2,0,2,0,0,0,0,0,2,0,2,0,0,0,0},
      {0,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,0},
      {0,2,0,0,2,0,0,0,2,0,2,0,0,0,2,0,0,2,0},
      {0,2,2,0,2,2,2,2,2,0,2,2,2,2,2,0,2,2,0},
      {0,0,2,0,2,0,2,0,0,0,0,0,2,0,2,0,2,0,0},
      {0,2,2,2,2,0,2,2,2,0,2,2,2,0,2,2,2,2,0},
      {0,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,2,0},
      {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
  t_duch d1 = { .poz_x = 8, .poz_y = 9, .ruch=rand() % 4};
  t_duch d2 = { .poz_x = 9, .poz_y = 9, .ruch=rand() % 4};
  t_duch d3 = { .poz_x = 10, .poz_y = 9, .ruch=rand() % 4};
  t_duch d4 = { .poz_x = 9, .poz_y = 7, .ruch=rand() % 4};
  t_gracz pacman = { .poz_x = 9, .poz_y = 15, .ruch=W_PRAWO, .monety=0};
  int zycie = 1;
  bool koniec = false;
  char c;
  int wynik;
  char nick[11]={""};
  rozpocznij_gre(&d1, &d2, &d3, &d4, &pacman, plansza, &wynik, nick);
  system("clear");
  pokaz_menu(wynik, nick);
  pokaz_plansze(plansza,monety);
  while(zycie != 0 && koniec == false && pacman.monety < ILOSC_MONET) {
    c=getch();
    if(c=='q')
      koniec = true;
    system("clear");
    zmien_kierunek_p(&pacman, plansza, c);
    steruj_pacmanem(&pacman, plansza, monety, &zycie);
    steruj_duchem(&d1, plansza, &zycie);
    steruj_duchem(&d2, plansza, &zycie);
    steruj_duchem(&d3, plansza, &zycie);
    steruj_duchem(&d4, plansza, &zycie);
    pokaz_menu(wynik, nick);
    pokaz_plansze(plansza,monety);

  }
  if(koniec == false) {
    zapisz_lepszy_wynik(wynik, pacman.monety);
  }

  return 0;
}

//**********Wyswietlanie elementow*************
void pokaz_plansze(int t[][SZER], int m[][SZER]) {
  for (int i=0; i<WYS; i++) {
    for (int j=0; j<SZER; j++) {
      if(m[i][j]==2 && t[i][j]==0)
        printf("\u00B7"); //moneta
      else
      switch (t[i][j]) {
          case 0: printf(" "); break; //puste pole
          case 1: printf(BLU "\u2588" RESET); break; //sciany planszy
          case 3: printf(RED "\u0394" RESET); break; //duch
          //PACMAN w zaleznosci od kierunku
          case 4: printf(YEL "V" RESET); break; //gora
          case 5: printf(YEL "\u039B" RESET); break; //dol
          case 6: printf(YEL ">" RESET); break; //prawo
          case 7: printf(YEL "<" RESET); break; //lewo
        }
    }
    printf("\n");
  }
}

void pokaz_menu(int wyn, char nick[]) {
  if(wyn > 0)
    printf("w,a,s,d - ruchy\t\tNajlepszy wynik:\nq - wyjscie\t\t%d - %s\n", wyn, nick);
  else
    printf("w,a,s,d - ruchy\nq - wyjscie\n");
}

//**********Sterowanie duchem******************
void wstaw_ducha(t_duch *d, int t[][SZER]) {
  t[d->poz_y][d->poz_x] = 3;
}

void przesun_ducha(t_duch *d) {
  switch(d->ruch)  {
    case W_GORE: (d->poz_y)--; break;
    case W_PRAWO: (d->poz_x)++; break;
    case W_DOL: (d->poz_y)++; break;
    case W_LEWO: (d->poz_x)--; break;
  }
}

void cofnij_ducha(t_duch *d) {
  switch(d->ruch)  {
    case W_GORE: (d->poz_y)++; break;
    case W_PRAWO: (d->poz_x)--; break;
    case W_DOL: (d->poz_y)--; break;
    case W_LEWO: (d->poz_x)++; break;
  }
}

bool czy_sciana(t_duch *d, int t[][SZER]) {
  if(t[d->poz_y][d->poz_x] == 1)
    return true;
  else
    return false;
}

bool czy_gracz(t_duch *d, int t[][SZER]) {
  if(t[d->poz_y][d->poz_x] == 4 || t[d->poz_y][d->poz_x] == 5 || t[d->poz_y][d->poz_x] == 6 || t[d->poz_y][d->poz_x] == 7)
    return true;
  else
    return false;
}

void zmien_kierunek_d(t_duch *d, int t[][SZER]) {
  bool kierunki[4] = {false, false, false, false}; //gora, prawo, lewo, dol
  int x = d->poz_x;
  int y = d->poz_y;
  int nowyk;
  if(t[y-1][x] != 1) kierunki[0] = true;
  if(t[y][x+1] != 1) kierunki[1] = true;
  if(t[y+1][x] != 1) kierunki[2] = true;
  if(t[y][x-1] != 1) kierunki[3] = true;
  nowyk = rand() % 4;
  while(kierunki[nowyk]== false)
    nowyk = rand() % 4;
  d->ruch = nowyk;
}

void usun_poprzedniego_d(t_duch *d, int t[][SZER]) {
  int x = d->poz_x;
  int y = d->poz_y;
  switch(d->ruch)  {
    case W_GORE: if(t[y+1][x]==3) t[y+1][x]=0;
      break;
    case W_PRAWO: if(t[y][x-1]==3) t[y][x-1]=0;
      break;
    case W_DOL: if(t[y-1][x]==3) t[y-1][x]=0;
      break;
    case W_LEWO: if(t[y][x+1]==3) t[y][x+1]=0;
      break;
  }
}

void steruj_duchem(t_duch *d, int t[][SZER], int *zycie) {
  przesun_ducha(d);
  if(czy_gracz(d, t)) {
    (*zycie)--;
  }
  if(czy_sciana( d, t)) {
    cofnij_ducha(d);
    zmien_kierunek_d(d, t);
    przesun_ducha(d);
  }
  usun_poprzedniego_d(d, t);
  wstaw_ducha(d, t);

}
//**********Sterowanie Pacmanem****************
void wstaw_pacmana(t_gracz *g, int t[][SZER]) {
  switch(g->ruch)  {
    case W_GORE: t[g->poz_y][g->poz_x] = 4; break;
    case W_PRAWO: t[g->poz_y][g->poz_x] = 7; break;
    case W_DOL: t[g->poz_y][g->poz_x] = 5; break;
    case W_LEWO: t[g->poz_y][g->poz_x] = 6; break;
  }
}

bool czy_moneta(t_gracz *g, int m[][SZER]) {
  if(m[g->poz_y][g->poz_x]==2)
    return true;
  else
    return false;
}

bool czy_duch(t_gracz *g, int t[][SZER]) {
  if(t[g->poz_y][g->poz_x]==3)
    return true;
  else
    return false;
}

char getch() {
  system ("/bin/stty raw");
  char ret = getchar();
  system ("/bin/stty cooked");
  return ret;
}

void przesun_pacmana(t_gracz *g, int t[][SZER]) {
  int x = g->poz_x;
  int y = g->poz_y;
  switch(g->ruch) {
    case W_GORE: if(t[y-1][x]!=1) (g->poz_y)--; break;
    case W_PRAWO: if(t[y][x+1]!=1) (g->poz_x)++; break;
    case W_DOL: if(t[y+1][x]!=1) (g->poz_y)++; break;
    case W_LEWO: if(t[y][x-1]!=1) (g->poz_x)--; break;
  }
}

void zmien_kierunek_p(t_gracz *g, int t[][SZER], char c) {
  int x = g->poz_x;
  int y = g->poz_y;
  switch(c) {
    case 'a': if(t[y][x-1]!=1) g->ruch=W_LEWO; break;
    case 'd': if(t[y][x+1]!=1) g->ruch=W_PRAWO; break;
    case 'w': if(t[y-1][x]!=1) g->ruch=W_GORE; break;
    case 's': if(t[y+1][x]!=1) g->ruch=W_DOL; break;
  }
}

void steruj_pacmanem(t_gracz *g, int t[][SZER], int m[][SZER], int *zycie) {
  int poprz_x=g->poz_x;
  int poprz_y=g->poz_y;
  przesun_pacmana(g, t);
  if(czy_duch(g, t))
    (*zycie)--;
  if(czy_moneta(g, m)) {
    (g->monety)++;
    m[g->poz_y][g->poz_x]=0;
  }
  if(poprz_x != g->poz_x || poprz_y != g->poz_y) {
    if(t[poprz_y][poprz_x] == 4 || t[poprz_y][poprz_x] == 5 || t[poprz_y][poprz_x] == 6 || t[poprz_y][poprz_x] == 7)
      t[poprz_y][poprz_x] = 0;
  }
  wstaw_pacmana(g,t);
}

//*************************************************
void rozpocznij_gre(t_duch *d1, t_duch *d2, t_duch *d3, t_duch *d4, t_gracz *g, int t[][SZER], int *wyn, char nick[]) {
  wstaw_ducha(d1, t);
  wstaw_ducha(d2, t);
  wstaw_ducha(d3, t);
  wstaw_ducha(d4, t);
  wstaw_pacmana(g, t);
  wczytaj_wynik(wyn, nick);
}
//***********Wczytywanie i zapis wyniku
void wczytaj_wynik(int *wyn, char nick[]) {
  FILE *f = fopen("wynik.txt", "r");
  if (f == NULL)
    *wyn = 0;
  else {
    fscanf(f, "%d", wyn);
    fscanf(f, "%s", nick);
    fclose(f);
  }
}

void zapisz_wynik(int wyn, char nick[]) {
  FILE *f = fopen("wynik.txt", "w");
  if (f == NULL)
    printf("blad zapisu\n");
  else {
    fprintf(f, "%d\n", wyn);
    fprintf(f, "%s", nick);
    fclose(f);
  }
}

void zapisz_lepszy_wynik(int wyn, int obecnyw) {
  if(obecnyw > wyn) {
    system("clear");
    printf("Gartulacje zdobyles %d monet.\nPodaj swoj nick(maks 10 znakow): ", obecnyw);
    char nick[11];
    scanf("%10s", nick);
    zapisz_wynik(obecnyw, nick);
  }
}
