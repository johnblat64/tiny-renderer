#include "matrix.h"
#include "v3d.h"
#include <inttypes.h>

v3d m2v(TransformMatrix tm){
    v3d v(
        tm._[0][0]/tm._[3][0],
        tm._[1][0]/tm._[3][0],
        tm._[2][0]/tm._[3][0]
    );
    return v;
}

TransformMatrix genIdTransformMatrix(){
    TransformMatrix tm = {0};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tm._[i][j] = (i==j ? 1.0f : 0.0f);
        }
    }
    return tm;
}


TransformMatrix operator*(TransformMatrix lhs, TransformMatrix rhs){
    TransformMatrix result = {0};
    const uint32_t n = 4;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            result._[i][j] = 0.0f;
            for(int k = 0; k < n; k++){
                result._[i][j] += lhs._[i][k] * rhs._[k][j];
            }
        }
    }
    return result;
}


v4d operator*(TransformMatrix lhs, v4d rhs){
    // TransformMatrix result;
    v4d result;
    const uint32_t n = 4;
    for(int i = 0; i < n; i++){
        result[i] = 0.0f;
        for(int k = 0; k < n; k++){
            result[i]+= lhs._[i][k] * rhs[k];
        }
    }
    return result;
}




// float cofactor(TransformMatrix m, uint32_t row, uint32_t col){

// }

// TransformMatrix adjugate(){
//     TransformMatrix result;
//     for(int i = 4; i >= 0; i--){
//         for(int j = 4; j >= 0; j--){
//             result[i][j]
//         }
//     }
// }


v3d operator*(UVMatrix lhs, v3d rhs){
    // TransformMatrix result;
    v3d result;
    const uint32_t nRows = 2;
    const uint32_t nCols = 3;
    for(int i = 0; i < nRows; i++){
        result[i] = 0.0f;
        for(int k = 0; k < nCols; k++){
            result[i]+= lhs._[i][k] * rhs[k];
        }
    }
    return result;
}
