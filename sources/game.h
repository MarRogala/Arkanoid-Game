#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct Game;

struct Point {
    double x, y;
}; // punkt

struct Sec {
    struct Point a, b;
}; // odcinek

struct Stra{
    double A, B, C;
}; // prosta w postaci ogolnej

typedef struct Point Point;
typedef struct Sec Sec;
typedef struct Stra Stra;

int pop_blo; // indeks ostatnio odbitego bloku
int total; // liczba blokow w poziomie

int next_blo; // indeks nastepnego bloku do odbicie
Sec next_sec; // odcinek od ktorego odbije sie pilka

int Play_lvl(struct Game *game, int x); // funkcja zajmujaca sie wywolaniem danego poziomu

int Check_play_events(struct Game *game, int *mn, double vx, double vy, int l); // sprawdzenie eventow podczas gry

void DrawLineInLvl(struct Game *game, int l); // rysuje linie na ekranie gry

void DrawTextInLvl(struct Game *game, int score, int x); // wypisuje tekst na ekranie gry

int Stop_Game(); // zatrzymuje gry po nacisnieciu P

double Gety(double x); // zwraca y na okragu podczas celowania

void Colision(struct Game *game, int x); // wyrywa kolizje

bool Game_Over(struct Game *game); // sprawedza czy gra sie skonczyla

void Show_Blocks(struct Game *game, int x); // wyswietla bloki danego poziomu

void Show_Ball(struct Game *game); // funkcja wyswietlajaca pilke

void Reset_Lvl(struct Game *game, int x); // przywraca wytrzymalosc blokow do stanu poczatkowego

void Bounce(Sec s, struct Game *game); // odbija pilke

void Fix(struct Game *game); // poprawia predkosc poruszania sie pilki

void Move_All_Blocks_Down(struct Game *game); // funkcja przesuwająca w dol wszystkie bloki

int Get_Rand(int a, int b); // funkcja zwracajaca losowa liczba z przedzialu [a, b]

void Add_Block(struct Game *game); // dodaje blok do gry

int Find_Low(struct Game *game); // znajduje dolny blok
