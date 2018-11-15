#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct Game;
typedef struct Game Game;

void Show_main_menu(Game *game); // funkcja wyswietlajaca glowne menu

int Check_events_main_menu(Game *game); // funkcja sprawdzajaca eventy w stonie z opisem

int Show_lvl_menu(Game *game); // funkcja wyswietlajaca karte wyboru poziomow

int Check_lvl_events(Game *game); // sprawdza eventy w karcie wyboru poziomow

int Show_ins_menu(Game *game); // wyswietla karte opisu

int Check_ins_events(Game *game); // sprawdze eventy w karcie opisu

int Show_wyn_menu(Game *game); // wyswietla karte wynikow

int Check_wyn_events(Game *game); // sprawdza eventy w karie wynikow
