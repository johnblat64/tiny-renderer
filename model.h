#ifndef model_h
#define model_h

#include "v3d.h"
#include <SDL2/SDL.h>

typedef struct Face {
    Uint32 i0, i1, i2;
} Face;

typedef struct Model {
    v3d *da_vertices;
    Face *da_faces;
} Model;

Model modelInit(const char *filename);


#endif