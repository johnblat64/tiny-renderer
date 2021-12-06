#ifndef matrix_h
#define matrix_h

#include "stb_ds.h"
#include "v3d.h"
#include "v4d.h"

// struct Matrix{   

//     float **arr;
//     size_t nRows;
//     size_t nCols;
    

//     Matrix(uint32_t r, uint32_t c) : nRows(r), nCols(c) {

//         float v = 0.0f;
//         size_t allocationSize = (sizeof(float) * r * c);
//         // arr = (float **)memoryArenaAllocate(&gMemoryArena, sizeof(float *) * r);
//         // arr[0] = (float *)memoryArenaAllocate(&gMemoryArena, sizeof(float) * c * r);
//         arr = (float **)memoryStackAllocate(&gMemoryStack, sizeof(float *) * r);
//         arr[0] = (float *)memoryStackAllocate(&gMemoryStack, sizeof(float) * c * r);
//         for(int i = 0; i < r; i++){
//             arr[i] = arr[0] + i * c;
//         }

//     }

//     ~Matrix(){
//         memoryStackFree(&gMemoryStack, arr);
//         nCols = 0;
//         nRows = 0;
//     }

//     float * &operator[](const uint32_t i){
//         return arr[i];
//     } 

//     uint32_t rows(){
//         return nRows;
//     }
//     uint32_t cols(){
//         return nCols;
//     }
// };

// Matrix genIdMatrix(int n){
//     Matrix m(n, n);
//     for(int i = 0; i < n; i++){
//         for(int j = 0; j < n; j++){
//             m[i][j] = (i==j ? 1.0f : 0.0f);
//         }
//     }
//     return m;
// }

// Matrix operator*(Matrix lhs, Matrix rhs){
//     if(lhs.nCols != rhs.nRows){
//         assert(0 && "lhs.nCols != rhs.nRows in matrix multiplication");
//     }

//     Matrix result(lhs.nRows, rhs.nCols);
//     for(int i = 0; i < lhs.nRows; i++){
//         for(int j = 0; j < rhs.nCols; j++){
//             result[i][j] = 0.0f;
//             for(int k = 0; k < lhs.nCols; k++){
//                 result[i][j] += lhs[i][k] * rhs[k][j];
//             }
//         }
//     }

//     return result;
// }

// Matrix operator*(Matrix lhs, v4d rhs){
//     if(lhs.nCols != 4){
//         assert(0 && "lhs.nCols != v4d 4 rows in matrix multiplication");
//     }

//     Matrix result(lhs.nRows, 1);
//     for(int i = 0; i < lhs.nRows; i++){
//         result[i][0] = 0.0f;
//         for(int k = 0; k < lhs.nCols; k++){
//             result[i][0] += lhs[i][k] * rhs[k];
//         }
//     }

//     return result;
// }

extern float *g_zBuffer;

struct TransformMatrix{
    float _[4][4];

    // float * &operator[](const uint32_t i){

    //     return &_[i];
    // } 
};



v3d m2v(TransformMatrix tm);
TransformMatrix genIdTransformMatrix();
TransformMatrix operator*(TransformMatrix lhs, TransformMatrix rhs);
v4d operator*(TransformMatrix lhs, v4d rhs);




#endif