#ifndef v3d_h
#define v3d_h

#include <math.h>
#include <inttypes.h>
#include <assert.h>

#define PI 3.14159265358979323846


/**
 * HEADER ONLY
 */
struct v3d {
    float x, y, z;

    v3d(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    v3d() : x(0), y(0), z(0) {}

    float &operator[](const uint32_t i){
        assert(i >= 0 && i <= 2);
        return 
            (i == 0) ?
            x : 
            (i == 1 ) ?
            y :
            z;
    }   

};


extern inline v3d v3d_add(v3d a, v3d b);
extern inline v3d v3d_sub(v3d a, v3d b);
extern inline v3d v3d_scale(float val, v3d a);
extern inline float v3d_dot( v3d a, v3d b );
extern inline float v3d_magnitude( v3d v );
extern inline float v3d_magnitude_noroot( v3d v );
extern inline v3d v3d_unit( v3d v );
extern inline v3d v3d_perp(v3d v);
extern inline float v3d_angle_between(v3d v, v3d u);
extern inline v3d v3d_rotate(v3d v, v3d o, float rads);
extern inline v3d v3d_cross(v3d v, v3d u);

#define v3d_normal v3d_unit


/**
 * IMPLEMENTATION DETAILS
 */
#ifdef V3D_IMPLEMENTATION

v3d v3d_add(v3d a, v3d b){
    return (v3d){a.x+b.x, a.y+b.y, a.z+b.z};
}


v3d v3d_sub(v3d a, v3d b){
    return (v3d){a.x-b.x, a.y-b.y, a.z-b.z};
}


v3d v3d_scale(float val, v3d a){
    return (v3d){a.x*val, a.y*val, a.z*val};
}


float v3d_dot( v3d a, v3d b ){
    return (float)((a.x*b.x)+(a.y*b.y)+(a.z*b.z));
}


float v3d_magnitude( v3d v ) {
    return sqrtf(v3d_dot(v, v));
}


float v3d_magnitude_noroot( v3d v ){
    return v3d_dot(v, v);

}


v3d v3d_unit( v3d v ) {
    v3d unit_vector;
    float magnitude = v3d_magnitude( v );

    unit_vector.x = (float)v.x / magnitude;
    unit_vector.y = (float)v.y / magnitude;
    unit_vector.z = (float)v.z / magnitude;

    return unit_vector;
}

// ????
v3d v3d_rotate(v3d v, v3d o, float rads){
    v3d rotatedV;
    float s = sin(rads);
    float c = cos(rads);
    v3d subtracted = v3d_sub(v, o);
    rotatedV.x = subtracted.x * c - subtracted.y * s;
    rotatedV.y = subtracted.x * s + subtracted.y * c;
    return rotatedV;
}

// ????
v3d v3d_perp(v3d v){
    v3d perpV = v3d_rotate(v, v3d(0,0,0), -(PI * 0.5));
    return perpV;
}


v3d v3d_cross(v3d v, v3d u){
    v3d crossProduct = {
        v.y*u.z - v.z*u.y,
        v.z*u.x - v.x*u.z,
        v.x*u.y - v.y*u.x
    };
    return crossProduct;
}


/**
 * OVERLOADS
 */

inline v3d operator+(const v3d lhs, const v3d rhs){
     return v3d_add(lhs, rhs);
}

inline v3d operator-(v3d lhs, v3d rhs){
    return v3d_sub(lhs, rhs);
}

inline v3d operator*(double lhs, v3d rhs){
    return v3d_scale(lhs, rhs);
}
   
inline v3d operator*(v3d lhs, double rhs){
    return v3d_scale(rhs, lhs);
}

inline float operator*(v3d lhs, v3d rhs){
    return v3d_dot(lhs, rhs);
}



#endif

#endif