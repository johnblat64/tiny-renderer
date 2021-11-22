#define V2D_IMPLEMENTATION
#include "v2d.h"
#include <stdio.h>

int main(){
    int x = 1;
    int y = 2;
    v2d v = v2d{(float)x,(float)y};
    v2d u = {1,2};
    float s = 5;
    v2d w = v * s;

    printf("%f, %f\n", w.x, w.y);
}