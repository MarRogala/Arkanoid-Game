#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

struct Block {
    double x, y; // lewy wierzcholek
    int life; // ile wytrzymalosci zostalo
    int type; // typ 1 - romb, 2 - gora, 3 - dol
};

struct Game {
    int res[12], total[12]; // tablica z najlepszymi wynikami
    struct Block Blocks[12][300]; // bloki dla kazdego poziomu

    double px, py; // polozenie platformy, rozmiary to w = 50, h = 10
    double bx, by; // polozenie pilki rozmiary 10 x 10
    double kx, ky; // x i y na okragu przed startem
    double vx, vy; // wektor pilki
    bool start, first;
    bool move_left;
    bool move_right;
    int life;

    SDL_Renderer *renderer;

    TTF_Font *font;

    Mix_Chunk *button_effect; // efekt przechodzenia do nastepnej karty

    SDL_Texture *mainmenu_texture; // obrazki na tlo menu
    SDL_Texture *lvlmenu_texture;
    SDL_Texture *insmenu_texture;
    SDL_Texture *wynmenu_texture;

    SDL_Texture *platform;
    SDL_Texture *ball;
    SDL_Texture *type1;
    SDL_Texture *type2;
    SDL_Texture *type3;
}; // główna struktura przechowujaca informacje o grze

void Load_gra(struct Game *game); // laduje grafiki

void Destroy_game(struct Game *game); // niszczy zadeklarowane struktury grafiki itp

void Inicialization(); // funkcja inicjalizujaca wszystkie biblioteki
