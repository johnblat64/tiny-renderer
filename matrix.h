#ifndef matrix_h
#define matrix_h

#include "stb_ds.h"
#include "memory.h"

struct Matrix{   

    float **arr;
    size_t nRows;
    size_t nCols;
    

    Matrix(uint32_t r, uint32_t c) : nRows(r), nCols(c) {

        float v = 0.0f;
        size_t allocationSize = (sizeof(float) * r * c);
        arr = (float **)memoryArenaAllocate(&gMemoryArena, sizeof(float *) * r);
        arr[0] = (float *)memoryArenaAllocate(&gMemoryArena, sizeof(float) * c * r);
        for(int i = 0; i < r; i++){
            arr[i] = arr[0] + i * c;
        }

    }

    float * &operator[](const uint32_t i){
        return arr[i];
    } 

    uint32_t rows(){
        return nRows;
    }
    uint32_t cols(){
        return nCols;
    }
};

Matrix genIdMatrix(int n){
    Matrix m(n, n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            m[i][j] = (i==j ? 1.0f : 0.0f);
        }
    }
    return m;
}

Matrix operator*(Matrix lhs, Matrix rhs){
    if(lhs.nCols != rhs.nRows){
        assert(0 && "lhs.nCols != rhs.nRows in matrix multiplication");
    }

    Matrix result(lhs.nRows, rhs.nCols);
    for(int i = 0; i < lhs.nRows; i++){
        for(int j = 0; j < rhs.nCols; j++){
            result[i][j] = 0.0f;
            for(int k = 0; k < lhs.nCols; k++){
                result[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }

    return result;
}


#endif