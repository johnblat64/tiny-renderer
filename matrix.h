#ifndef matrix_h
#define matrix_h

#include "stb_ds.h"
#include "memory.h"

struct Matrix{   
    struct row {
        /**
         * @brief This internal struct just serves
         * as a error prevention tactic by checking
         * the bounds of the array
         * 
         */
        float *da;
        size_t size;
        
        row(uint32_t n){
            da = NULL;
            float v = 0;
            for(int i = 0; i < n; i++)
                stbds_arrpush(da, v);
        }

        float &operator[](const uint32_t i){
            assert(i >= 0 && i < stbds_arrlen(da));
            return da[i];
        } 
    };
    /**
     * @brief 
     * ACTUAL MATRIX
     * 
     * It uses stb_ds behind the scenes, 
     * so calling stbds_arrlen() on either of the arrays will give you 
     * the sizes
     * 
     */

    float *arr;
    size_t nRows;
    size_t nCols;
    

    Matrix(uint32_t r, uint32_t c) : nRows(r), nCols(c) {

        float v = 0.0f;
        size_t allocationSize = (sizeof(float) * r * c);
        arr = (float *)memoryArenaAllocate(&gMemoryArena, allocationSize);
        // memset(arr, 0, allocationSize);
        // da_rows = NULL;
        
        // for(int r = 0; r < nRows; r++){
        //     row aRow = row(nCols);            
        //     stbds_arrpush(da_rows, aRow);
        // }
    }

    float *operator[](const uint32_t i){
        float *ptr = arr+i;
        return ptr;
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



#endif