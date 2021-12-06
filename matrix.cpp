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
    TransformMatrix result;
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
