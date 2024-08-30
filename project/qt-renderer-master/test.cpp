#include <stdio.h>
#include <chrono>
int64_t getTimestamp() {
	auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
/*
alpha > 0 && beta > 0 && gamma 
alpha = ((y-ya)(xc-xa)-(x-xa)(yc-ya))/((yb-ya)(xc-xa)-(xb-xa)(yc-ya))
beta = ((y-ya)(xb-xa)-(x-xa)(yb-ya))/((yc-ya)(xb-xa)-(xc-xa)(yb-ya))
gamma = 1-alpha-beta

ya/yb/yc
xa/xb/xc
    0,1 a
-1,0    1,0
b       c
*/
float x,y;
float ay,by,cy,ax,bx,cx;
float Ia, Ib, Ic;

float t1,t2,t3;
float x1,x2,x3;
float I1, I2, I3;
float insert(float x, float y) {
    t1 = (y-by)/(ay-by);
    x1 = bx + t1*(ax-bx);
    I1 = Ib + t1*(Ia-Ib);

    t2 = (y-cy)/(ay-cy);
    x2 = cx + t2*(ax-cx);
    I2 = Ic + t2*(Ia-Ic);

    t3 = (x-x1)/(x2-x1);
    x3 = x1 + t3*(x2-x1);
    I3 = I1 + t3*(I2-I1);

    // printf("t1 %3.2f, t2 %3.2f, t3 %3.2f\n", t1, t2, t3);
    // printf("I1 %3.2f, I2 %3.2f, I3 %3.2f\n", I1, I2, I3);
    // printf("x1 %3.2f, x2 %3.2f, x3 %3.2f\n", x1, x2, x3);


    return 0;
}
float alpha, beta, gamma;
float I;
void computeBarycentric2D(float x, float y) {
/*
    alpha = ((y-ya)(xc-xa)-(x-xa)(yc-ya))/((yb-ya)(xc-xa)-(xb-xa)(yc-ya))
    beta =  ((y-ya)(xb-xa)-(x-xa)(yb-ya))/((yc-ya)(xb-xa)-(xc-xa)(yb-ya))
*/
    beta = ((y-ay)*(cx-ax)-(x-ax)*(cy-ay))/((by-ay)*(cx-ax)-(bx-ax)*(cy-ay));
    gamma  = ((y-ay)*(bx-ax)-(x-ax)*(by-ay))/((cy-ay)*(bx-ax)-(cx-ax)*(by-ay));
    alpha = 1 - gamma - beta;
    I = alpha * Ia + beta * Ib + gamma * Ic;
    // printf("%3.2f/%3.2f/%3.2f\n", alpha, beta, gamma);
    // printf("I=%3.2f\n", I);
}
int main()
{
    Ia = 10;
    Ib = 20;
    Ic = 30;

    ax = 0;
    ay = 1;

    bx = -1;
    by = 0;

    cx = 1;
    cy = 0;
    int i;
    int count = 1000000000;


    float alpha, beta, gamma;
    float Za, Zb, Zc;
    x = 0;
    y = 0.1f;
    int64_t begin =  getTimestamp();
    for(i=0;i<count;i++)
        insert(x, y);
    int64_t end1 =  getTimestamp();
    for(i=0;i<count;i++)
        computeBarycentric2D(x, y);
    int64_t end2 =  getTimestamp();
    printf("%d ms, %d ms\n", end1-begin, end2-end1);
    
    printf("out\n");
    return 0;
}