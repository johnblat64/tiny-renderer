#ifndef model_h
#define model_h

#include "v3d.h"
#include "v2d.h"
#include <SDL2/SDL.h>

typedef struct Face {
    Uint32 i0, i1, i2;

    Uint32 &operator[](const Uint32 i){
        assert(i >= 0 && i <= 2);
        return
            (i == 0) ?
            i0 :
            (i == 1) ?
            i1 :
            i2;
    }
} Face;

struct Model {
    v3d *da_vertices;
    Face *da_faces;
    v3d *da_normals;
    Face *da_normalFaces;
    v2d *da_textureCoordinates;
    Face *da_textureFaces;
    SDL_Color *da_texture;
    Uint32 texture_w;
    Uint32 texture_h;
};

Model modelInit(const char *filename);


#endif