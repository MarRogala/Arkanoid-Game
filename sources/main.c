/*
    Projek WDPC Arkanoid Marcin Rogala 2017/1018
    Glowny modul projektu
    - wyczytuje wszystkie grafiki
    - inicjalizuje biblioteki zewnętrzne
    - wywoluje funkcje wyswietlajaca menu glowne
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "menu.h"
#include "main.h"
#include "dane.h"
#include "game.h"

static const int width = 800;
static const int height = 600;

void Inicialization() // funkcja inicjalizujaca wszystkie biblioteki
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("SDL_Init failed: %s\n", SDL_GetError());
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        printf("Img_Init failed: %s\n", IMG_GetError());
    if(TTF_Init() < 0)
        printf("TTF_Init failed: %s\n", TTF_GetError());
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
        printf("Mix_OpenAudio failed %s\n", Mix_GetError());
    srand(time(NULL));
}

void Load_gra(Game *game)
{
    SDL_Surface *mainmenu = IMG_Load("../img/mainmenu2.jpg");
    game->mainmenu_texture = SDL_CreateTextureFromSurface(game->renderer, mainmenu);
    SDL_Surface *lvlmenu = IMG_Load("../img/lvlmenu.jpg");
    game->lvlmenu_texture = SDL_CreateTextureFromSurface(game->renderer, lvlmenu);
    SDL_Surface *insmenu = IMG_Load("../img/instrukcjamenu.jpg");
    game->insmenu_texture = SDL_CreateTextureFromSurface(game->renderer, insmenu);
    SDL_Surface *wynmenu = IMG_Load("../img/naj_wyniki.jpg");
    game->wynmenu_texture = SDL_CreateTextureFromSurface(game->renderer, wynmenu);

    SDL_Surface *ballsur = IMG_Load("../img/ball.png");
    game->ball = SDL_CreateTextureFromSurface(game->renderer, ballsur);
    SDL_Surface *platsur = IMG_Load("../img/platform.jpg");
    game->platform = SDL_CreateTextureFromSurface(game->renderer, platsur);
    SDL_Surface *type1sur = IMG_Load("../img/typ1.png");
    game->type1 = SDL_CreateTextureFromSurface(game->renderer, type1sur);
    SDL_Surface *type2sur = IMG_Load("../img/typ2.png");
    game->type2 = SDL_CreateTextureFromSurface(game->renderer, type2sur);
    SDL_Surface *type3sur = IMG_Load("../img/typ3.png");
    game->type3 = SDL_CreateTextureFromSurface(game->renderer, type3sur);

    SDL_FreeSurface(mainmenu);
    SDL_FreeSurface(lvlmenu);
    SDL_FreeSurface(insmenu);
    SDL_FreeSurface(wynmenu);
    SDL_FreeSurface(ballsur);
    SDL_FreeSurface(platsur);
    SDL_FreeSurface(type1sur);
    SDL_FreeSurface(type2sur);
    SDL_FreeSurface(type3sur);
}

void Destroy_game(Game *game)
{
    SDL_DestroyTexture(game->mainmenu_texture);
    SDL_DestroyTexture(game->lvlmenu_texture);
    SDL_DestroyTexture(game->wynmenu_texture);
    SDL_DestroyTexture(game->insmenu_texture);
    SDL_DestroyTexture(game->platform);
    SDL_DestroyTexture(game->ball);
    SDL_DestroyTexture(game->type1);
    SDL_DestroyTexture(game->type2);
    SDL_DestroyTexture(game->type3);
}

int main()
{
    Inicialization();

    Game game;

    SDL_Window *window = SDL_CreateWindow("ARKANOID", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Mix_Chunk *button_effect = Mix_LoadWAV("Blip_Select.wav");

    game.renderer = renderer;
    game.button_effect = button_effect;

    Mix_VolumeChunk(game.button_effect, MIX_MAX_VOLUME / 8);

    TTF_Font *font = TTF_OpenFont("whitrabt.ttf", 32);
    game.font = font;

    Load_game(&game);

    Load_gra(&game);

    int Gameover = 0;

    while(!Gameover)
    {
        Show_main_menu(&game);
        int next = Check_events_main_menu(&game);
        if(next == 5) Gameover = 1;
    }

    Save_game(&game);

    Destroy_game(&game);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    Mix_FreeChunk(button_effect);

    atexit(SDL_Quit);
    atexit(IMG_Quit);
    atexit(Mix_Quit);

    return 0;
}
