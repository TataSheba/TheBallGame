#include "TXLib.h"
#include <stdlib.h>
#include <math.h>

//const double radius = 35;

void MoveBall ();

void DriveBall (double* vx, double* vy);

void Physics (double* x, double* y, double* vx, double* vy, double radius, double dt);

void DrawBall (double x, double y, double vx, double vy, double radius, COLORREF color);

void myClear( COLORREF color);

COLORREF MySetColor ();

double Distance (double x1, double y1, double x2, double y2);

int Collision (double x1, double y1, double v1, double r1,double x2, double y2, double v2, double r2);

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
    double radius1 = 30;
    double x1  = radius1 + (double)(rand()%(700 - (int)radius1 + 1));
    double y1  = radius1 + (double)(rand()%(550 - (int)radius1 + 1));
    double vx1 = (double)(50-rand()%100+1);
    double vy1 = (double)(50-rand()%100+1);
    double dt1 = 0.4;
    COLORREF colorBall_1 = TX_ORANGE;

    double radius2 = 25;
    double x2  = radius2 + (double)(rand()%(700 - (int)radius2 + 1));
    double y2  = radius2 + (double)(rand()%(550 - (int)radius2 + 1));
    double vx2 = (double)(50-rand()%100+1);
    double vy2 = (double)(50-rand()%100+1);
    double dt2 = 0.4;
    COLORREF colorBall_2 = MySetColor();

    double radius3 = 15;
    double x3  = radius3 + (double)(rand()%(700 - (int)radius3 + 1));
    double y3  = radius3 + (double)(rand()%(550 - (int)radius3 + 1));
    double vx3 = (double)(50-rand()%100+1);
    double vy3 = (double)(50-rand()%100+1);
    double dt3 = 0.4;
    COLORREF colorBall_3 = MySetColor();

    double radius4 = 20;
    double x4  = radius4 + (double)(rand()%(700 - (int)radius4 + 1));
    double y4  = radius4 + (double)(rand()%(550 - (int)radius4 + 1));
    double vx4 = (double)(50-rand()%100+1);
    double vy4 = (double)(50-rand()%100+1);
    double dt4 = 0.4;
    COLORREF colorBall_4 = MySetColor();

    //int count=0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        myClear(TX_BLACK);

        DriveBall(&vx1, &vy1);
        Physics (&x1, &y1, &vx1, &vy1, radius1, dt1);
        DrawBall (x1, y1, 0, 0, radius1, colorBall_1);

        Physics (&x2, &y2, &vx2, &vy2, radius2, dt2);
        DrawBall (x2, y2, vx2, vy2, radius2, colorBall_2);

        Physics (&x3, &y3, &vx3, &vy3, radius3, dt3);
        DrawBall (x3, y3, vx3, vy3, radius3, colorBall_3);

        Physics (&x4, &y4, &vx4, &vy4, radius4, dt4);
        DrawBall (x4, y4, vx4, vy4, radius4, colorBall_4);

        //&&&


        txSleep (5);
        }
    }

//---- Функция управления скоростью шарика----
void DriveBall (double* vx, double* vy)
    {
    if (!txGetAsyncKeyState (VK_UP))
        *vy+= 3;
    if (!txGetAsyncKeyState (VK_DOWN))
        *vy-= 3;
    if (!txGetAsyncKeyState (VK_RIGHT))
        *vx+= 3;
    if (!txGetAsyncKeyState (VK_LEFT))
        *vx-= 3;
    }

//----Рисование шарика без смены цвета------
void Physics (double* x, double* y, double* vx, double* vy, double radius, double dt)
    {
    double xx  =  *x;
    double yy  =  *y;
    double vxx = *vx;
    double vyy = *vy;

    vxx = vxx*0.99;
    vyy = vyy*0.99;
    xx += vxx*dt;
    yy += vyy*dt;

    if ((xx >= 800 - radius) || (xx <= radius))
        {
        vxx  = -vxx;
        xx  +=  vxx*dt;
        };

     if ((yy >= 600 - radius) || (yy <= radius))
        {
        vyy = -vyy;
        yy +=  vyy*dt;
        };

    *x = xx;
    *y = yy;
    *vx = vxx;
    *vy = vyy;
    }

//---------Управление скоростью шарика---------

void DrawBall(double x, double y, double vx, double vy, double radius, COLORREF color)
    {
    txSetFillColor (color);
    txCircle (x, y, radius);
    //--- вектор скорости----
    txSetColor(TX_YELLOW, 3);
    txLine(x, y, x+vx, y+vy);
    //--- проекция VX ----
    if (vx> 0)
        txSetColor(TX_PINK,2);
        else
    txSetColor (TX_CYAN, 2);
    txLine(x, y, x + vx, y);
    //--- проекция VY ----
    if (vy> 0)
        txSetColor(TX_ORANGE, 2);
        else
    txSetColor (TX_CYAN, 2);
    txLine(x, y, x, y+vy);
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
double Distance (double x1, double y1, double x2, double y2)
    {
    double dist = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

    return dist;
    }

//------- Функция обработки поведения при столкновении шариков------
//--------Если V2/2 < V1< 2*V2, то 2й шарик исчезает и игрок получает +1 балл-----
//--------Иначе шарики отталкиваются и игрок получает -1 балл---------------------
//---------Если игрок набрал -10 баллов, то игра останавливается с фразой "Проиграл!"----
void Collision (int* count,
               double x1, double y1, double* vvx1, double* vvy1, double r1,
               double x2, double y2, double* vvx2, double* vvy2, double r2,
               double x3, double y3, double* vvx3, double* vvy3, double r3,
               double x4, double y4, double* vvx4, double* vvy4, double r4)
    {
    double d12, d13, d14, d23, d24, d34;
    d12 = Distance (x1, y1, x2, y2);
    d13 = Distance (x1, y1, x3, y3);
    d14 = Distance (x1, y1, x4, y4);
    d23 = Distance (x2, y2, x3, y3);
    d24 = Distance (x2, y2, x4, y4);
    d34 = Distance (x3, y3, x4, y4);

    double vvv;
    int bonus = *count;

    double vx1 = *vvx1;
    double vy1 = *vvy1;
    double vx2 = *vvx2;
    double vy2 = *vvy2;
    double vx3 = *vvx3;
    double vy3 = *vvy3;
    double vx4 = *vvx4;
    double vy4 = *vvy4;


         //--- 1 VS 2---
    if (d12 <= r1 + r2)
        {
        if (fabs (vx1 + vx2) <= r1 + r2)
            {
            vvv = vx1;
            vx1  = vx2;
            vx2  = vvv;
            }
            else
                {
                vvv = vy1;
                vy1  = vy2;
                vy2  = vvv;
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
            }
            else
                {
                vvv = vy1;
                vy1 = vy3;
                vy3 = vvv;
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
            }
            else
                {
                vvv = vy1;
                vy1  = vy4;
                vy4  = vvv;
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
        };


    }
