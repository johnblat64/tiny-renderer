#ifndef v3d_h
#define v3d_h

#include <math.h>

#define PI 3.14159265358979323846


typedef struct v3d {
    float x, y, z;
} v3d;


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

#define v3d_normal v3d_unit

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
    v3d perpV = v3d_rotate(v, (v3d){0,0}, -(PI * 0.5));
    return perpV;
}


#endif

#endif