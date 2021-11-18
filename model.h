#ifndef model_h
#define model_h

#include "v3d.h"
#include <SDL2/SDL.h>

typedef struct Model {
    v3d *da_vertices;
    Uint32 **da_faces;
} Model;

Model modelInit(const char *filename);


#endif