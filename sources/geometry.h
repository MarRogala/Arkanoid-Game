#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct Game;

double Dist(Point a, Point B); // zwraca odleglosc miedzy dwoma punktami

bool Check_cros(Sec sec, Stra stra); // czy prosta przecina sie z odcinkiem

double Fx(Stra stra, double x); // zwraca wartosc na prostej w punkcie x

Stra Make_stra(Point a, Point b); // zwraca prosta na ktrej leza dwa punkty

Point Cros_point(Stra stra1, Stra stra2); // zwraca punkt przeciecia odcinka i prostej

double SecDist(Sec s, Point p, struct Game *game, int f); // zwraca odleglosc punktu od odcinka

bool Check_point_in_sec(Point p, Sec s); // spradza czy punkt zawiera sie w odcinku

Stra Perp(Stra s, Point p); // zwraca prosta prostopadla do s przechodzaca przez p

bool Check_side(Point p, struct Game *game); // spradza czy punkt lezy po dobrej stronie - czy pilka sie do niego zbliza

Sec Find_sec(struct Game *game, int l); // znajduje najblizszy odcinek bloku dla polozenia pilki

bool Check_dist(struct Game *game); // spradza czy pilka zblizyla sie do odcinka
