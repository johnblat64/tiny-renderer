#define V2D_IMPLEMENTATION
#include "v2d.h"
#include <stdio.h>
#include "matrix.h"
#define V4D_IMPLEMENTATION
#include "v4d.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int main(){
    Matrix m = Matrix(4,4);

    m[3][3] = 5.0f;

    printf("0,0: %f\n", m[3][2]);

}