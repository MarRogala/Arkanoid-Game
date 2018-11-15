#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

extern FILE *REK;
extern FILE *LVL;

struct Game;

void Save_game(Game *game); // zapisuje najlepsze wyniki do plike

void Load_game(Game *game); // wczytuje dane z plikow

void Reset_Best(Game *game); // resetuje nalepsze wyniki
