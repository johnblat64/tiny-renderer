#ifndef v2d_h
#define v2d_h

#include <math.h>

#define PI 3.14159265358979323846


typedef struct v2d {
    union{
        struct {float x, y;};
        float raw[2];
    };
} v2d;

typedef struct v2di {
    int x, y;
} v2di;


extern inline v2d v2d_add(v2d a, v2d b);
extern inline v2d v2d_sub(v2d a, v2d b);
extern inline v2d v2d_scale(float val, v2d a);
extern inline float v2d_dot( v2d a, v2d b );
extern inline float v2d_magnitude( v2d v );
extern inline float v2d_magnitude_noroot( v2d v );
extern inline v2d v2d_unit( v2d v );
extern inline v2d v2d_perp(v2d v);
extern inline float v2d_angle_between(v2d v, v2d u);
extern inline v2d v2d_rotate(v2d v, v2d o, float rads);

#define v2d_normal v2d_unit

#ifdef V2d_IMPLEMENTATION

v2d v2d_add(v2d a, v2d b){
    return (v2d){a.x+b.x, a.y+b.y};
}


v2d v2d_sub(v2d a, v2d b){
    return (v2d){a.x-b.x, a.y-b.y};
}


v2d v2d_scale(float val, v2d a){
    return (v2d){a.x*val, a.y*val};
}


float v2d_dot( v2d a, v2d b ){
    return (float)((a.x*b.x)+(a.y*b.y));
}


float v2d_magnitude( v2d v ) {
    return sqrtf(v2d_dot(v, v));
}


float v2d_magnitude_noroot( v2d v ){
    return v2d_dot(v, v);

}


v2d v2d_unit( v2d v ) {
    v2d unit_vector;
    float magnitude = v2d_magnitude( v );

    unit_vector.x = (float)v.x / magnitude;
    unit_vector.y = (float)v.y / magnitude;


    return unit_vector;
}


v2d v2d_rotate(v2d v, v2d o, float rads){
    v2d rotatedV;
    float s = sin(rads);
    float c = cos(rads);
    v2d subtracted = v2d_sub(v, o);
    rotatedV.x = subtracted.x * c - subtracted.y * s;
    rotatedV.y = subtracted.x * s + subtracted.y * c;
    return rotatedV;
}


v2d v2d_perp(v2d v){
    v2d perpV = v2d_rotate(v, (v2d){0,0}, -(PI * 0.5));
    return perpV;
}

#endif



#endif