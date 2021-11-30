#ifndef v4d_h
#define v4d_h

#include <math.h>
#include <inttypes.h>
#include <assert.h>

#define PI 3.14159265358979323846


/**
 * HEADER ONLY
 */
struct v4d {
    float x, y, z, w;

    v4d(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}
    v4d() : x(0), y(0), z(0), w(0) {}

    float &operator[](const uint32_t i){
        assert(i >= 0 && i <= 3);
        return 
            (i == 0) ?
            x : 
            (i == 1 ) ?
            y :
            (i == 2) ?
            z :
            w;
    }   

};


extern inline v4d v4d_add(v4d a, v4d b);
extern inline v4d v4d_sub(v4d a, v4d b);
extern inline v4d v4d_scale(float val, v4d a);
extern inline float v4d_dot( v4d a, v4d b );
extern inline float v4d_magnitude( v4d v );
extern inline float v4d_magnitude_noroot( v4d v );
extern inline v4d v4d_unit( v4d v );
extern inline v4d v4d_perp(v4d v);
extern inline float v4d_angle_between(v4d v, v4d u);
extern inline v4d v4d_rotate(v4d v, v4d o, float rads);
extern inline v4d v4d_cross(v4d v, v4d u);

#define v4d_normal v4d_unit


/**
 * IMPLEMENTATION DETAILS
 */
#ifdef v4d_IMPLEMENTATION

v4d v4d_add(v4d a, v4d b){
    return (v4d){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}


v4d v4d_sub(v4d a, v4d b){
    return (v4d){a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}


v4d v4d_scale(float val, v4d a){
    return (v4d){a.x*val, a.y*val, a.z*val, a.w*val};
}


float v4d_dot( v4d a, v4d b ){
    return (float)((a.x*b.x)+(a.y*b.y)+(a.z*b.z)+(a.w*b.w));
}


float v4d_magnitude( v4d v ) {
    return sqrtf(v4d_dot(v, v));
}


float v4d_magnitude_noroot( v4d v ){
    return v4d_dot(v, v);

}


v4d v4d_unit( v4d v ) {
    v4d unit_vector;
    float magnitude = v4d_magnitude( v );

    unit_vector.x = (float)v.x / magnitude;
    unit_vector.y = (float)v.y / magnitude;
    unit_vector.z = (float)v.z / magnitude;
    unit_vector.w = (float)v.w / magnitude;

    return unit_vector;
}

// ????
v4d v4d_rotate(v4d v, v4d o, float rads){
    v4d rotatedV;
    float s = sin(rads);
    float c = cos(rads);
    v4d subtracted = v4d_sub(v, o);
    rotatedV.x = subtracted.x * c - subtracted.y * s;
    rotatedV.y = subtracted.x * s + subtracted.y * c;
    return rotatedV;
}

// ????
v4d v4d_perp(v4d v){
    v4d perpV = v4d_rotate(v, v4d(0,0,0), -(PI * 0.5));
    return perpV;
}


// v4d v4d_cross(v4d v, v4d u){
//     v4d crossProduct = {
//         v.y*u.z - v.z*u.y,
//         v.z*u.x - v.x*u.z,
//         v.x*u.y - v.y*u.x,
//         v.w*u
//     };
//     return crossProduct;
// }


/**
 * OVERLOADS
 */

inline v4d operator+(const v4d lhs, const v4d rhs){
     return v4d_add(lhs, rhs);
}

inline v4d operator-(v4d lhs, v4d rhs){
    return v4d_sub(lhs, rhs);
}

inline v4d operator*(double lhs, v4d rhs){
    return v4d_scale(lhs, rhs);
}
   
inline v4d operator*(v4d lhs, double rhs){
    return v4d_scale(rhs, lhs);
}

inline float operator*(v4d lhs, v4d rhs){
    return v4d_dot(lhs, rhs);
}



#endif

#endif