
#ifndef r_h
#define r_h

#include <inttypes.h>
#include "matrix.h"
#include "v2d.h"
#include "v3d.h"
#include <SDL2/SDL.h>
#include "model.h"

void R_putPixel(v2d worldPoint, Uint32 color);
TransformMatrix genViewportTransformMatrix(int x, int y, int w, int h);
TransformMatrix lookAt(v3d eye, v3d center, v3d up);
void R_drawTriangleTextured(v4d *vertices, SDL_Color *texture, Uint32 texture_w, v2di *textureCoords);

extern v3d gouradVaryingIntensity;
v4d gouradVertexShader(uint32_t iface, uint32_t nthvert, Model *model);
bool gouradFragmentShader(v3d bar, SDL_Color *color);



#endif