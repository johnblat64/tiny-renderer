#define V2D_IMPLEMENTATION
#include "v2d.h"
#include <stdio.h>

int main(){
    int x = 1;
    int y = 2;
    v2d v = v2d(x,y);
    v2d u = v2d(1,2);
    float s = 5;
    v2d w = v * s;
    v[0] = 5;

    printf("%f, %f\n", w.x, w.y);
}