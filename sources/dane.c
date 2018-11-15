/*
    Projek WDPC Arkanoid Marcin Rogala 2017/1018
    Moduł danych
    - wczytuje najlepsze wyniki z pliku tekstowego rekordy.textTexture
    - wczytuje opis bolokow dla wszystkich poziomow z pliki poziomy.txt
    - zapisuje nowe rekordy
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "menu.h"
#include "dane.h"

FILE *REK;
FILE *LVL;

void Load_game(Game *game)
{
    REK = fopen("rekordy.txt", "r");
    LVL = fopen("poziomy.txt", "r");
    for(int i = 1; i <= 10; i++)
        fscanf(REK, "%d", &game->res[i]);
    for(int i = 1; i <= 9; i++)
    {
        fscanf(LVL, "%d", &game->total[i]);
        for(int j = 1; j <= game->total[i]; j++)
        {
            double x, y;
            int type;
            fscanf(LVL, "%d %lf %lf", &type, &x, &y);
            game->Blocks[i][j].x = x;
            game->Blocks[i][j].y = y;
            game->Blocks[i][j].type = type;
            if(type == 3) game->Blocks[i][j].life = 2;
            else game->Blocks[i][j].life = 1;
        }
    }
    fclose(REK);
    fclose(LVL);
}

void Save_game(Game *game)
{
    REK = fopen("rekordy.txt", "w");
    for(int i = 1; i <= 10; i++)
    {
        char *t = malloc(100 * sizeof(char));
        sprintf(t, "%d", game->res[i]);
        fprintf(REK, "%s\n", t);
    }
    fclose(REK);
}

void Reset_Best(Game *game)
{
    for(int i = 1; i <= 9; i ++)
        game->res[i] = 1000000007;
    game->res[10] = 0;
    Save_game(game);
}
