#include "TXLib.h"
#include <stdlib.h>
#include <math.h>

struct BALL
    {
    double x;
    double y;
    double vx;
    double vy;
    double r;
    double dt;
    int red;
    int green;
    int blue;
    };

void MoveBall ();

void DriveBall (BALL* s);

void Physics (BALL* s);

void DrawBall (BALL s);

void myClear (COLORREF color);

COLORREF MySetColor ();

double Distance (BALL s1, BALL s2);

void Collision (int* count1, BALL* s1, BALL* s2, BALL* s3, BALL* s4);

double InitCoord (double radius);
double InitSpeed ();

int main()
    {
    txBegin();
    txCreateWindow (800, 600);

    MoveBall();

    txEnd();
    return 0;
    }

//---- движение шарика ----
void MoveBall ()
    {
    int prize=0;
    double rad = 30;
    BALL b1 = {
               rad,
               InitCoord (rad), InitCoord (rad),
               InitSpeed (), InitSpeed(),
               0.4,
               250, 180,  40
               };

    rad = 35;
    BALL b2 = {rad,
               InitCoord (rad), InitCoord (rad),
               InitSpeed (), InitSpeed(),
               0.4,
               rand()%245,
               rand()%245,
               rand()%245
               };

    rad = 45;
    BALL b3 = {
               rad,
               InitCoord (rad), InitCoord (rad),
               InitSpeed (), InitSpeed(),
               0.4,
               rand()%245,
               rand()%245,
               rand()%245
               };


    rad = 43;
    BALL b4 = {
               rad,
               InitCoord (rad), InitCoord (rad),
               InitSpeed (), InitSpeed(),
               0.4,
               rand()%250,
               rand()%250,
               rand()%250
               };

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        myClear(TX_BLACK);

        DriveBall(&(b1));
        Physics (&b1);
        DrawBall (b1);

        Physics (&b2);
        DrawBall (b2);

        Physics (&b3);
        DrawBall (b3);

        Physics (&b4);
        DrawBall (b4);

        Collision (&prize, &b1, &b2, &b3, &b4);

        txTextOut(10, 10, "Ваши баллы: ");

        txSleep (5);
        }
    }

//---- Функция управления скоростью шарика----
void DriveBall (BALL* s)
    {
    if (!txGetAsyncKeyState (VK_UP))
        (*s).vy+= 3;
    if (!txGetAsyncKeyState (VK_DOWN))
        (*s).vy-= 3;
    if (!txGetAsyncKeyState (VK_RIGHT))
        (*s).vx+= 3;
    if (!txGetAsyncKeyState (VK_LEFT))
        (*s).vx-= 3;
    }

//----Рисование шарика без смены цвета------
void Physics (BALL* s)
    {
    double xx  = (*s).x;
    double yy  = (*s).y;
    double vxx = (*s).vx;
    double vyy = (*s).vy;
    double radius = (*s).r;
    double d = (*s).dt;

    vxx = vxx*0.99;
    vyy = vyy*0.99;
    xx += vxx*d;
    yy += vyy*d;

    if (xx >= (800 - radius) || xx <= radius )
        {
        vxx  = -vxx;
        xx  +=  vxx*d;
        };

     if (yy >= (600 - radius) || yy <= radius )
        {
        vyy = -vyy;
        yy +=  vyy*d;
        };

    (*s).x = xx;
    (*s).y = yy;
    (*s).vx = vxx;
    (*s).vy = vyy;
    }

//---------Управление скоростью шарика---------

void DrawBall(BALL s)
    {
    int rr = s.red;
    int gg = s.green;
    int bb = s.blue;
    txSetColor(RGB(rr, gg, bb));
    txSetFillColor(RGB(rr, gg, bb));
    txCircle (s.x, s.y, s.r);
    //--- вектор скорости----
    txSetColor(TX_YELLOW, 3);
    txLine(s.x, s.y, s.x + s.vx, s.y + s.vy);
    //--- проекция VX ----
    if (s.vx> 0)
        txSetColor(TX_PINK,2);
        else
    txSetColor (TX_CYAN, 2);
    txLine(s.x, s.y, s.x + s.vx, s.y);
    //--- проекция VY ----
    if (s.vy > 0)
        txSetColor(TX_ORANGE, 2);
        else
    txSetColor (TX_CYAN, 2);
    txLine(s.x, s.y, s.x, s.y + s.vy);
    //--- возврат настроек цвета-----
    txSetColor (TX_BLACK);
    }


//-----Очистка перед перерисовкой шарика-----
void myClear(COLORREF color)
    {
    txSetFillColor(color);
    txRectangle (0,0, 800, 600);
    }

//------ Задание случайного цвета-------------
COLORREF MySetColor ()
    {
    int red   = rand()%250;
    int green = rand()%250;
    int blue  = rand()%250;
    int color = RGB (red, green, blue);
    return color;
    }

//-------Расчет расстояния между 2 шариками----------
double Distance (BALL s1, BALL s2)
    {
    double dist = sqrt((s1.x-s2.x)*(s1.x-s2.x)+(s1.y-s2.y)*(s1.y-s2.y));

    return dist;
    }

//------- Функция обработки поведения при столкновении шариков------
//--------Если V2/2 < V1< 2*V2, то 2й шарик исчезает и игрок получает +1 балл-----
//--------Иначе шарики отталкиваются и игрок получает -1 балл---------------------
//---------Если игрок набрал -10 баллов, то игра останавливается с фразой "Проиграл!"----
void Collision (int* count1, BALL* s1, BALL* s2, BALL* s3, BALL* s4)
    {
    double d12, d13, d14, d23, d24, d34;
    d12 = Distance (*s1, *s2);
    d13 = Distance (*s1, *s3);
    d14 = Distance (*s1, *s4);
    d23 = Distance (*s2, *s3);
    d24 = Distance (*s2, *s4);
    d34 = Distance (*s3, *s4);

    double vvv;
    int bonus = *count1;

    double vx1 = (*s1).vx;
    double vy1 = (*s1).vy;
    double vx2 = (*s2).vx;
    double vy2 = (*s2).vy;
    double vx3 = (*s3).vx;
    double vy3 = (*s3).vy;
    double vx4 = (*s4).vx;
    double vy4 = (*s4).vy;
    double r1  = (*s1).r;
    double r2  = (*s2).r;
    double r3  = (*s3).r;
    double r4  = (*s4).r;


         //--- 1 VS 2---
    if (d12 <= r1 + r2)
        {
        if (fabs (vx1 + vx2) <= r1 + r2)
            {
            vvv = vx1;
            vx1  = vx2;
            vx2  = vvv;
            bonus++;
            }
            else
                {
                vvv = vy1;
                vy1  = vy2;
                vy2  = vvv;
                bonus++;
                }
        };
         //--- 1 VS 3----
    if (d13 <= r1 + r3)
        {
        if (fabs (vx1 + vx3) <= r1 + r3)
            {
            vvv = vx1;
            vx1  = vx3;
            vx3  = vvv;
            bonus++;
            }
            else
                {
                vvv = vy1;
                vy1 = vy3;
                vy3 = vvv;
                bonus++;
                }
        };
        //---- 1 VS 4------
    if (d14 <= r1 + r4)
        {
        if (fabs(vx1 + vx4) <= r1 + r4)
            {
            vvv = vx1;
            vx1  = vx4;
            vx4  = vvv;
            bonus++;
            }
            else
                {
                vvv = vy1;
                vy1  = vy4;
                vy4  = vvv;
                bonus++;
                }
        };
        //----2 VS 3----
    if (d23 <= r2 + r3)
        {
        if (fabs(vx2 + vx3) <= r2 + r3)
            {
            vvv = vx2;
            vx2  = vx3;
            vx3  = vvv;
            }
            else
                {
                vvv = vy2;
                vy2  = vy3;
                vy3  = vvv;
                }
        };
        //----2 VS 4----
    if (d24 <= r2 + r4)
        {
        if (fabs (vx2 + vx4) <= r2 + r4)
            {
            vvv = vx2;
            vx2 = vx4;
            vx4 = vvv;
            }
            else
                {
                vvv = vy2;
                vy2 = vy4;
                vy4 = vvv;
                }
        };
        //----3 VS 4----
    if (d34 <= r3 + r4)
        {
        if (fabs (vx3 + vx4) <= r3 + r4)
            {
            vvv = vx3;
            vx3  = vx4;
            vx4  = vvv;
            }
            else
                {
                vvv = vy3;
                vy3  = vy4;
                vy4  = vvv;
                }
        }

    (*s1).vx = vx1;
    (*s1).vy = vy1;
    (*s2).vx = vx2;
    (*s2).vy = vy2;
    (*s3).vx = vx3;
    (*s3).vy = vy3;
    (*s4).vx = vx4;
    (*s4).vy = vy4;
    }

double InitCoord (double radius)
    {
    double cc = radius + (double)(rand()%(700 - (int)(radius + 1)));
    return cc;
    }

double InitSpeed ()
    {
    double cc = (double)(50 - rand()%100 + 1);
    return cc;
    }
