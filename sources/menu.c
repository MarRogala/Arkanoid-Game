/*
    Projek WDPC Arkanoid Marcin Rogala 2017/1018
    Moduł zajmujacy sie wyswietlaniem i obslugiwaniem kart mainmenu
    - wyswietla grafiki w kartach menu
    - spradza eventy w kartach
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
#include "game.h"
#include "dane.h"

const int INF = 1e9 + 7;

int Show_wyn_menu(Game *game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_Rect wynmenu_pic;
    wynmenu_pic.x = 0, wynmenu_pic.y = 0, wynmenu_pic.w = 800, wynmenu_pic.h = 600;

    int wid = 270, wid2 = 445, w = 0, h = 0;
    int hig[] = {0, 118, 155, 192, 230, 267, 303, 337, 376, 413, 445};
    char *t = malloc(20 * sizeof(char)), *b = "Brak wyniku!";

    SDL_Color color = {153, 153, 153, 255};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture[12];
    SDL_Rect textRect[12];

    for(int i = 1; i <= 9; i++)
    {
        sprintf(t, "%d", game->res[i]);
        if(game->res[i] == INF) textSurface = TTF_RenderText_Solid(game->font, b, color);
        else textSurface = TTF_RenderText_Solid(game->font, t, color);
        textTexture[i] = SDL_CreateTextureFromSurface(game->renderer, textSurface);
        textRect[i].x = wid, textRect[i].y = hig[i];
        if(game->res[i] == INF) TTF_SizeText(game->font, b, &w, &h);
        else TTF_SizeText(game->font, t, &w, &h);
        textRect[i].h = h, textRect[i].w = w;
    }

    sprintf(t, "%d", game->res[10]);
    if(game->res[10] == 0) textSurface = TTF_RenderText_Solid(game->font, b, color);
    else textSurface = TTF_RenderText_Solid(game->font, t, color);
    textTexture[10] = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    textRect[10].x = wid2, textRect[10].y = hig[10];
    if(game->res[10] == 0) TTF_SizeText(game->font, b, &w, &h);
    else TTF_SizeText(game->font, t, &w, &h);
    textRect[10].w = w, textRect[10].h = h;

    SDL_FreeSurface(textSurface);

    int back = 0;
    while(!back)
    {
        SDL_RenderCopy(game->renderer, game->wynmenu_texture, NULL, &wynmenu_pic);

        SDL_RenderCopy(game->renderer, textTexture[1], NULL, &textRect[1]);
        SDL_RenderCopy(game->renderer, textTexture[2], NULL, &textRect[2]);
        SDL_RenderCopy(game->renderer, textTexture[3], NULL, &textRect[3]);
        SDL_RenderCopy(game->renderer, textTexture[4], NULL, &textRect[4]);
        SDL_RenderCopy(game->renderer, textTexture[5], NULL, &textRect[5]);
        SDL_RenderCopy(game->renderer, textTexture[6], NULL, &textRect[6]);
        SDL_RenderCopy(game->renderer, textTexture[7], NULL, &textRect[7]);
        SDL_RenderCopy(game->renderer, textTexture[8], NULL, &textRect[8]);
        SDL_RenderCopy(game->renderer, textTexture[9], NULL, &textRect[9]);
        SDL_RenderCopy(game->renderer, textTexture[10], NULL, &textRect[10]);

        SDL_RenderPresent(game->renderer);
        back = Check_wyn_events(game);
        SDL_Delay(0);
    }
    return back;
}

int Check_wyn_events(Game *game)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            return 2;
        if(event.type == SDL_KEYDOWN)
        {
            //Mix_PlayChannel(-1, game->button_effect, 0);
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return 1;
        }
        if(event.key.keysym.sym == SDLK_r)
        {
            Reset_Best(game);
            return 1;
        }
    }
    return 0;
}

int Show_ins_menu(Game *game)
{
    SDL_Rect insmenu_pic;
    insmenu_pic.x = 0, insmenu_pic.y = 0, insmenu_pic.w = 800, insmenu_pic.h = 600;

    int back = 0;
    while(!back)
    {
        SDL_RenderCopy(game->renderer, game->insmenu_texture, NULL, &insmenu_pic);
        SDL_RenderPresent(game->renderer);
        back = Check_ins_events(game);
    }
    return back;
}

int Check_ins_events(Game *game)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            return 2;
        if(event.type == SDL_KEYDOWN)
        {
            //Mix_PlayChannel(-1, game->button_effect, 0);
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return 1;
        }
    }
    return 0;
}

int Show_lvl_menu(Game *game)
{
    SDL_Rect lvlmenu_pic;
    lvlmenu_pic.x = 0, lvlmenu_pic.y = 0, lvlmenu_pic.w = 800, lvlmenu_pic.h = 600;

    int back = 0;
    while(!back)
    {
        SDL_RenderCopy(game->renderer, game->lvlmenu_texture, NULL, &lvlmenu_pic);
        SDL_RenderPresent(game->renderer);
        back = Check_lvl_events(game);
    }
    return back;
}

int Check_lvl_events(Game *game)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            return 2;
        if(event.type == SDL_KEYDOWN)
        {
            //Mix_PlayChannel(-1, game->button_effect, 0);
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    return 1;
                    continue;
                }
                case SDLK_1:
                {
                    int x = Play_lvl(game, 1);
                    if(x != INF)
                        if(game->res[1] > x) game->res[1] = x;
                    Reset_Lvl(game, 1);
                    continue;
                }
                case SDLK_2:
                {
                    int x = Play_lvl(game, 2);
                    if(x != INF)
                        if(game->res[2] > x) game->res[2] = x;
                    Reset_Lvl(game, 2);
                    continue;
                }
                case SDLK_3:
                {
                    int x = Play_lvl(game, 3);
                    if(x != INF)
                        if(game->res[3] > x) game->res[3] = x;
                    Reset_Lvl(game, 3);
                    continue;
                }
                case SDLK_4:
                {
                    int x = Play_lvl(game, 4);
                    if(x != INF)
                        if(game->res[4] > x) game->res[4] = x;
                    Reset_Lvl(game, 4);
                    continue;
                }
                case SDLK_5:
                {
                    int x = Play_lvl(game, 5);
                    if(x != INF)
                        if(game->res[5] > x) game->res[5] = x;
                    Reset_Lvl(game, 5);
                    continue;
                }
                case SDLK_6:
                {
                    int x = Play_lvl(game, 6);
                    if(x != INF)
                        if(game->res[6] > x) game->res[6] = x;
                    Reset_Lvl(game, 6);
                    continue;
                }
                case SDLK_7:
                {
                    int x = Play_lvl(game, 7);
                    if(x != INF)
                        if(game->res[7] > x) game->res[7] = x;
                    Reset_Lvl(game, 7);
                    continue;
                }
                case SDLK_8:
                {
                    int x = Play_lvl(game, 8);
                    if(x != INF)
                        if(game->res[8] > x) game->res[8] = x;
                    Reset_Lvl(game, 8);
                    continue;
                }
                case SDLK_9:
                {
                    int x = Play_lvl(game, 9);
                    if(x != INF)
                        if(game->res[9] > x) game->res[9] = x;
                    Reset_Lvl(game, 9);
                    continue;
                }
            }
        }
    }
    return 0;
}


void Show_main_menu(Game *game)
{
    SDL_Rect main_pic;
    main_pic.x = 0, main_pic.y = 0, main_pic.w = 800, main_pic.h = 600;
    SDL_RenderCopy(game->renderer, game->mainmenu_texture, NULL, &main_pic);
    SDL_RenderPresent(game->renderer);
}

int Check_events_main_menu(Game *game)
{
    int res = 0;
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            return 5;
        else if(event.type == SDL_KEYDOWN)
        {
            //Mix_PlayChannel(-1, game->button_effect, 0);
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    return 5;
                    continue;
                    // wyjscie
                }
                case SDLK_1:
                {
                    //instrukcja
                    if(Show_ins_menu(game) == 2) return 5;
                    continue;
                }
                case SDLK_2:
                {
                    if(Show_lvl_menu(game) == 2) return 5;
                    continue;
                    //wybor poziomu
                }
                case SDLK_3:
                {
                    int x = Play_lvl(game, 10);
                    if(x != INF)
                        if(game->res[10] < x) game->res[10] = x;
                    Reset_Lvl(game, 10);
                    continue;
                    //nieskonczona gra
                }
                case SDLK_4:
                {
                    if(Show_wyn_menu(game) == 2) return 5;
                    continue;
                    //rekordy
                }
            }
        }
    }
    return res;
}
