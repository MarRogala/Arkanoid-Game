/*
    Projek WDPC Arkanoid Marcin Rogala 2017/1018
    Moduł zawierający funkcje geometryczne
    - znajduje najblizszy blok dla aktualnego polozenia i kierunku pilki
    - zawiera fukncje niezbedne do odbicia pilkie od bloku
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "game.h"
#include "geometry.h"

struct Game;

Sec Find_sec(struct Game *game, int l)
{
    Sec res;
    res.a.x = 1000000.0, res.a.y = 1000000.0, res.b.x = 1000000.0, res.b.y = 1000000.0;
    double mn = 1000000007.0;
    Point b1;
    b1.x = game->bx + 5.0, b1.y = game->by + 5.0;

    int tot = game->total[l];
    if(l == 10) tot = 20;
    for(int i = 1; i <= tot; i++)
    {
        int type = game->Blocks[l][i].type;
        if(game->Blocks[l][i].life <= 0) continue;
        if(type == 1)
        {
            Sec s1, s2, s3, s4;
            double x = game->Blocks[l][i].x, y = game->Blocks[l][i].y;
            s1.a.x = x, s1.a.y = y, s1.b.x = x + 20.0, s1.b.y = y - 30.0;
            s2.a.x = x, s2.a.y = y, s2.b.x = x + 20.0, s2.b.y = y + 30.0;
            s3.a.x = x + 20.0, s3.a.y = y - 30.0, s3.b.x = x + 40.0, s3.b.y = y;
            s4.a.x = x + 40.0, s4.a.y = y, s4.b.x = x + 20.0, s4.b.y = y + 30.0;
            double d1 = SecDist(s1, b1, game, 1), d2 = SecDist(s2, b1, game, 1), d3 = SecDist(s3, b1, game, 1), d4 = SecDist(s4, b1, game, 1);
            if(d1 < mn) mn = d1, res = s1, next_blo = i;
            if(d2 < mn) mn = d2, res = s2, next_blo = i;
            if(d3 < mn) mn = d3, res = s3, next_blo = i;
            if(d4 < mn) mn = d4, res = s4, next_blo = i;
        }
        if(type == 2)
        {
            Sec s1, s2, s3;
            double x = game->Blocks[l][i].x, y = game->Blocks[l][i].y;
            s1.a.x = x , s1.a.y = y, s1.b.x = x + 20.0, s1.b.y = y - 30.0;
            s2.a.x = x + 20.0, s2.a.y = y - 30.0, s2.b.x = x + 40.0, s2.b.y = y;
            s3.a.x = x, s3.a.y = y, s3.b.x = x + 40.0, s3.b.y = y;
            double d1 = SecDist(s1, b1, game, 1), d2 = SecDist(s2, b1, game, 1), d3 = SecDist(s3, b1, game, 1);
            if(d1 < mn) mn = d1, res = s1, next_blo = i;
            if(d2 < mn) mn = d2, res = s2, next_blo = i;
            if(d3 < mn) mn = d3, res = s3, next_blo = i;
        }
        if(type == 3)
        {
            Sec s1, s2, s3;
            double x = game->Blocks[l][i].x, y = game->Blocks[l][i].y;
            s1.a.x = x , s1.a.y = y, s1.b.x = x + 20.0, s1.b.y = y + 30.0;
            s2.a.x = x + 40.0, s2.a.y = y, s2.b.x = x + 20.0, s2.b.y = y + 30.0;
            s3.a.x = x, s3.a.y = y, s3.b.x = x + 40.0, s3.b.y = y;
            double d1 = SecDist(s1, b1, game, 1), d2 = SecDist(s2, b1, game, 1), d3 = SecDist(s3, b1, game, 1);
            if(d1 < mn) mn = d1, res = s1, next_blo = i;
            if(d2 < mn) mn = d2, res = s2, next_blo = i;
            if(d3 < mn) mn = d3, res = s3, next_blo = i;
        }
    }
    return res;
}

void Fix(struct Game *game)
{
    while(sqrt(game->vx * game->vx + game->vy * game->vy) > 6.0)
    {
        game->vx /= 1.2;
        game->vy /= 1.2;
    }
    while(sqrt(game->vx * game->vx + game->vy * game->vy) < 5.0)
    {
        game->vx *= 1.2;
        game->vy *= 1.2;
    }
}

bool Check_dist(struct Game *game)
{
    Point b1;
    b1.x = game->bx + 5.0, b1.y = game->by + 5.0;
    double d = SecDist(next_sec, b1, game, 0);
    if(d < 3.5) return 1;
    else return 0;
}

bool Check_side(Point p, struct Game *game)
{
    double vx = game->vx, vy = game->vy;
    double bx = game->bx + 5.0, by = game->by + 5.0;
    if(vx != 0.0)
    {
        if(vx < 0.0 && p.x < bx) return 1;
        if(vx > 0.0 && p.x > bx) return 1;
    }
    if(vy != 0.0)
    {
        if(vy < 0.0 && p.y < by) return 1;
        if(vy > 0.0 && p.y > by) return 1;
    }
    return 0;
}

Stra Perp(Stra s, Point p)
{
    Stra res;
    s.A /= -s.B, s.C /= -s.B, s.B /= -s.B;
    res.A = -(1.0 / s.A);
    res.C = p.x * res.A - p.y * res.B;
    return res;
}

bool Check_point_in_sec(Point p, Sec s)
{
    if((p.x >= s.a.x && p.x <= s.b.x) || (p.x <= s.a.x && p.x >= s.b.x)) return 1;
    else return 0;
}

double SecDist(Sec s, Point p, struct Game *game, int f)
{
    Stra st = Make_stra(s.a, s.b);
    Point pp, cro;
    pp.x = p.x + game->vx, pp.y = p.y + game->vy;
    Stra st2 = Make_stra(p, pp);
    cro = Cros_point(st, st2);
    if(Check_side(cro, game) == 0 && f == 1) return 1000000008.0;
    if(Check_point_in_sec(cro, s))
        return Dist(p, cro);
    else
    return 1000000008.0;
}

Stra Make_stra(Point A, Point B)
{
    Stra res;
    res.A = A.y - B.y;
    res.B = B.x - A.x;
    res.C = B.y * A.x - A.y * B.x;
    return res;
}

double Dist(Point a, Point b)
{
    return (double)sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double Fx(Stra stra, double x)
{
    return ((stra.A * x + stra.C) / -stra.B);
}

bool Check_cros(Sec sec, Stra stra)
{
    if((sec.a.y >= Fx(stra, sec.a.x) & sec.b.y <= Fx(stra, sec.b.x)) || (sec.a.y <= Fx(stra, sec.a.x) & sec.b.y >= Fx(stra, sec.b.x)))
        return 1;
    else return 0;
}

Point Cros_point(Stra st1, Stra st2)
{
    Point res;
    if(st2.B == 0.0)
    {
        res.x = (-st2.C) / st2.A;
        res.y = ((-st1.C) - (st1.A * res.x)) / st1.B;
        return res;
    }
    st1.A /= -st1.B, st1.C /= -st1.B, st1.B /= -st1.B;
    st2.A /= -st2.B, st2.C /= -st2.B, st2.B /= -st2.B;
    res.x = (st1.C - st2.C) / (st2.A - st1.A);
    res.y = Fx(st1, res.x);
    return res;
}
