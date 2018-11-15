/*
    Projek WDPC Arkanoid Marcin Rogala 2017/1018
    Moduł zajmujący się rozgrywką
    - wyswietla bloki oraz inne elementy ekranu rozgrywki
    - wyswietla najlepsze wyniki
    - wykrywa kolicje
    - odbija pilke zmienajac jej wektor
    - zatrzymuje gre po nacisnieciu przycisku p
    - liczy punkty na podstawie czasu gry
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "main.h"
#include "menu.h"
#include "dane.h"
#include "geometry.h"

struct Game;

int Find_Low(Game *game)
{
    int mx = 0;
    for(int i = 1; i <= 20; i++)
    {
        if(game->Blocks[10][i].life > 0)
        {
            if(game->Blocks[10][i].y > mx)
                mx = game->Blocks[10][i].y;
        }
    }
    return mx;
}

void Add_Block(Game *game)
{
    double xc = (double)Get_Rand(100, 660);
    int type = Get_Rand(1, 3);
    int last = 1;
    while(game->Blocks[10][last].life > 0) last++;
    game->Blocks[10][last].y = 90.0;
    game->Blocks[10][last].x = xc;
    game->Blocks[10][last].type = type;
    int life = 1;
    if(type == 3) life = 2;
    game->Blocks[10][last].life = life;
    game->total[10] ++;
}

void Move_All_Blocks_Down(Game *game)
{
    for(int i = 1; i <= 20; i++)
        if(game->Blocks[10][i].life != 0) game->Blocks[10][i].y += 60.0;
}

int Get_Rand(int a, int b)
{
    return a + rand() % (b - a + 1);
}

void Bounce(Sec s, Game *game)
{
    if(s.a.y == s.b.y)
    {
        game->vy *= -1.0;
        return;
    }
    Point b1, b2;
    b1.x = game->bx + 5.0, b1.y = game->by + 5.0;
    b2.x = b1.x - game->vx, b2.y = b1.y - game->vy;
    Stra st1 = Make_stra(b1, b2); // prosta pilki
    Stra st2 = Make_stra(s.a, s.b); // prosta odcinka

    Point cro1 = Cros_point(st2, st1);
    game->bx = cro1.x, game->by = cro1.y;
    game->bx -= 5.0, game->by -= 5.0;

    Point to_ref;
    to_ref.x = cro1.x + game->vx, to_ref.y = cro1.y + game->vy;

    Stra st3 = Perp(st2, to_ref); // prosta prostopadla to prostej odcinka przechodzaca przez to_ref
    Point cro2 = Cros_point(st2, st3);
    double dx = cro2.x - to_ref.x, dy = cro2.y - to_ref.y;
    dx *= 2.0, dy *= 2.0;
    Point ref;
    ref.x = to_ref.x + dx, ref.y = to_ref.y + dy;
    game->vx = ref.x - cro1.x, game->vy = ref.y - cro1.y;
}

void Colision(Game *game, int l)
{
    Point b;
    b.x = game->bx, b.y = game->by;
    int bx = game->bx, by = game->by;
    if(bx <= 2.0 || bx >= 792.0) // sciany pionowe
    {
        game->vx *= -1.0;
        game->first = 1;
        if(pop_blo == -1 && game->vy * game->vy < 1.0)
        {
            if(game->vy < 0.0) game->vy -= 1.0;
            else game->vy += 1.0;
        }
        pop_blo = -1;
        next_sec = Find_sec(game, l);
    }
    if(by <= 55.0) //gorna sciana
    {
        game->vy *= -1.0;
        game->first = 1;
        pop_blo = -1;
        next_sec = Find_sec(game, l);
    }
    int plus1 = 0;
    if(game->move_right || game->move_left) plus1 = 10.0;
    if(by >= 535 && by <= 540 && game->first == 1) // platforma
        if(bx + 10.0 > game->px - plus1 && bx < game->px + 100.0 + plus1)
        {
            if(pop_blo == -1)
            {
                if(game->vx > 0.0 && game->vx < 0.5) game->vx += 0.3;
                if(game->vx < 0.0 && game->vx > 0.5) game->vx -= 0.3;
            }
            game->vy *= -1.0, game->first = 0, pop_blo = -1;
            if(game->vx > 0.0 && game->move_right) game->vx *= -1.0;
            else if(game->vx < 0.0 && game->move_left) game->vx *= -1.0;
            else if(game->vx > 0.0 && game->move_right) game->vy += 1.0;
            else if(game->vx < 0.0 && game->move_left) game->vy -= 1.0;
            Fix(game);
            next_sec = Find_sec(game, l);
        }
    if(Check_dist(game))
    {
        game->first = 1;
        game->Blocks[l][next_blo].life --;
        if(game->Blocks[l][next_blo].life == 0)
        {
            total--;
            if(l == 10) game->total[10]--;
        }
        Bounce(next_sec, game);
        pop_blo = next_blo;
        next_sec = Find_sec(game, l);
        if(next_sec.a.x == 1000000.0) next_blo = -1;
        Show_Ball(game);
    }
    Fix(game);
}

bool Game_Over(Game *game)
{
    if(game->by > 545.0)
    {
        game->life--;
        game->px = 350.0, game->py = 550.0;
        game->bx = 395.0, game->by = 540.0;
        game->vx = -0.0005, game->vy = -0.0001;
        game->start = 0;
        game->kx = 402.0, game -> ky = 460.0;
        game->first = 0;
    }
    if(game->life == 0) return 1;
    else return 0;
}

int Stop_Game()
{
    int restart = 0;
    SDL_Event event;

    int Tm = clock();
    int y = Tm;

    while(!restart)
    {
        Tm = (clock() - Tm) / 100000;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
                exit(0);
            if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                    {
                        return -1;
                        continue;
                    }
                    case SDLK_p:
                    {
                        restart = 1;
                        return Tm - y / 100000;
                    }
                }
            }
        }
    }
    return 0;
}

double Gety(double x)
{
    return -sqrt(6400.0 - (x - 400.0) * (x - 400.0)) + 540.0;
}

int Check_play_events(Game *game, int *mn, double vx, double vy, int l)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            exit(0);
        if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                {
                    return 1;
                }
                case SDLK_p:
                {
                    int x = 0;
                    x += Stop_Game();
                    if(x == -1) return 1;
                    *mn += x;
                    continue;
                }
                case SDLK_RETURN:
                {
                    game->vx = vx;
                    game->vy = vy;
                    game->start = 1;
                    next_sec = Find_sec(game, l);
                }
                break;
            }
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT])
    {
        game->move_left = 1;
        game->move_right = 0;
        if(game->start == 1)
        {
            if(game->px >= 10.0) game->px -= 10.0;
        }
        else
        {
            if(game->kx >= 330.0) game->kx -= 3.0;
            game->ky = Gety(game->kx);
        }
    }
    else if(state[SDL_SCANCODE_RIGHT])
    {
        game->move_left = 0;
        game->move_right = 1;
        if(game->start == 1)
        {
            if(game->px <= 690.0) game->px += 10.0;
        }
        else
        {
            if(game->kx <= 470.0) game->kx += 3.0;
            game->ky = Gety(game->kx);
        }
    }
    else game->move_left = 0, game->move_right = 0;
    return 0;
}

void DrawLineInLvl(Game *game, int l)
{
    SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(game->renderer, 0, 50, 800, 50);
    if(game->start == 0) SDL_RenderDrawLine(game->renderer, 400, 545, game->kx, game->ky);
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    if(l == 10) SDL_RenderDrawLine(game->renderer, 0, 510, 800, 510);
    SDL_SetRenderDrawColor(game->renderer, 20, 20, 20, 255);
    SDL_SetRenderDrawColor(game->renderer, 20, 20, 20, 255);
}

void DrawTextInLvl(Game *game, int score, int x)
{
    SDL_Color color = {153, 153, 153, 255};
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect textRect;

    char *t[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "Nieskonczona gra"};
    char *tt = malloc(100 * sizeof(char)), *sco = malloc(10 * sizeof(char));
    if(score < 0) score = 0;
    sprintf(sco, "%d", score);
    if(x != 10) sprintf(tt, "Poziom: %s Punkty: %s Pilek: %d", t[x - 1], sco, game->life);
    else sprintf(tt, "Nieskonczona gra Punkty: %s Pilek: %d", sco, game->life);

    int w, h;
    textSurface = TTF_RenderText_Solid(game->font, tt, color);
    textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    textRect.x = 15, textRect.y = 10;
    TTF_SizeText(game->font, tt, &w, &h);
    textRect.h = h, textRect.w = w;
    SDL_RenderCopy(game->renderer, textTexture, NULL, &textRect);
}

void Reset_Lvl(Game *game, int x)
{
    if(x == 10)
    {
        for(int i = 0; i <= 20; i++)
            game->Blocks[x][i].life = 0;
        game->total[10] = 0;
    }
    for(int i = 1; i <= game->total[x]; i++)
    {
        if(game->Blocks[x][i].type == 3) game->Blocks[x][i].life = 2;
        else game->Blocks[x][i].life = 1;
    }
}

void Show_Blocks(Game *game, int z)
{
    SDL_Rect BR[300];
    int tot = game->total[z];
    if(z == 10) tot = 10;
    for(int i = 1; i <= tot; i++)
    {
        if(game->Blocks[z][i].life > 0)
        {
            int x, y, w, h, ty = game->Blocks[z][i].type;
            if(ty == 1) x = game->Blocks[z][i].x, y = game->Blocks[z][i].y - 30.0, w = 40.0, h = 60.0;
            if(ty == 2) x = game->Blocks[z][i].x, y = game->Blocks[z][i].y - 30.0, w = 40.0, h = 30.0;
            if(ty == 3) x = game->Blocks[z][i].x, y = game->Blocks[z][i].y, w = 40.0, h = 30.0;
            BR[i].x = x, BR[i].y = y, BR[i].w = w, BR[i].h = h;
            if(ty == 1) SDL_RenderCopy(game->renderer, game->type1, NULL, &BR[i]);
            if(ty == 2) SDL_RenderCopy(game->renderer, game->type2, NULL, &BR[i]);
            if(ty == 3) SDL_RenderCopy(game->renderer, game->type3, NULL, &BR[i]);
        }
    }
}

void Show_Ball(struct Game *game)
{
    SDL_Rect ball;
    ball.w = 10.0, ball.h = 10.0;
    ball.x = game->bx, ball.y = game->by;
    SDL_RenderCopy(game->renderer, game->ball, NULL, &ball);
}


int Play_lvl(Game *game, int x) // zagraj poziom x, zwraca wynik
{
    if(x == 10)
    {
        Reset_Lvl(game, 10);
        game->total[x] = 0;
        for(int i = 1; i <= 4; i++)
            Move_All_Blocks_Down(game), Add_Block(game);
    }

    SDL_SetRenderDrawColor(game->renderer, 20, 20, 20, 255);

    game->px = 350.0, game->py = 550.0;
    game->bx = 395.0, game->by = 540.0;
    game->vx = -0.0005, game->vy = -0.0001;
    game->start = 0;
    game->kx = 402.0, game -> ky = 460.0;
    game->first = 0;
    pop_blo = -1;
    if(x == 7 || x == 8 || x == 9 || x == 10) game->life = 1;
    else game->life = 2;

    if(x == 10)
    {
        game->kx = (double)Get_Rand(350, 450);
        while(game->kx == 400.0) game->kx = (double)Get_Rand(350, 450);
        game->ky = Gety(game->kx);
        game->vx = (game->kx - 400.0) / 16.0;
        game->vy = (game->ky - 540.0) / 16.0;
        game->start = 1;
        next_sec = Find_sec(game, x);
    }

    int over = 0;
    SDL_Rect plat;
    plat.w = 100.0, plat.h = 10.0;

    int clock_s;
    clock_s = clock();

    int y = clock_s, mn = 0;
    double vx = 0.0, vy = 0.0;
    total = game->total[x];

    while(!over)
    {
        clock_s = (clock() - clock_s) / 100000;
        SDL_RenderClear(game->renderer);
        DrawTextInLvl(game, clock_s - y / 100000 - mn, x);
        DrawLineInLvl(game, x);

        vx = (game->kx - 400.0) / 16.0, vy = (game->ky - 540.0) / 16.0;
        over = Check_play_events(game, &mn, vx, vy, x);
        if(over == 1) return 1000000007;
        if(game->vx < 0.2 && game->vx > 0.0) game->vx = 0.2;
        if(game->vx > -0.2 && game->vx < 0.0) game->vx = -0.2;
        if(game->start == 1) game->bx += game->vx, game->by += game->vy;
        Colision(game, x);
        if(x != 10)
        {
            if(total == 0) break;
            if(Game_Over(game))
            {
                SDL_Delay(1000);
                return 1000000007;
            }
            next_sec = Find_sec(game, x);
        }
        else
        {
            int low = Find_Low(game);
            int ti = Get_Rand(1, 1001);
            if(ti % 1000 == 0 || low == 0)
            {
                Sec S;
                S.a.x = 1000000.0, S.a.y = 1000000.0, S.b.x = 1000000.0, S.b.y = 1000000.0;
                if(next_sec.a.x == S.a.x)
                {
                    Move_All_Blocks_Down(game), Add_Block(game);
                    next_sec = Find_sec(game, 10);
                }
            }
            if(Game_Over(game) || low > 470)
            {
                Show_Blocks(game, x);
                SDL_RenderPresent(game->renderer);
                SDL_Delay(1000);
                return clock_s - y / 100000 - mn;
            }
        }
        plat.x = game->px, plat.y = game->py;
        SDL_RenderCopy(game->renderer, game->platform, NULL, &plat);
        Show_Blocks(game, x);
        Show_Ball(game);
        SDL_RenderPresent(game->renderer);
    }
    game->start = 0;
    return clock_s - y / 100000 - mn;
}
