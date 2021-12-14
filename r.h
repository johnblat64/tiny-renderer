
#ifndef r_h
#define r_h

#include <inttypes.h>
#include "matrix.h"
#include "v2d.h"
#include "v3d.h"
#include <SDL2/SDL.h>
#include "model.h"

struct R_Shader{
    float gouradVaryingIntensity;
    v3d varyingUV; 
    TransformMatrix uniformMatrix; // Projection * ModelView
    TransformMatrix uniformMatrixInverseTranspose; // (Projection * ModelView) inverted and transposed
    v4d (* vertexShaderFn)(uint32_t iface, uint32_t nthvert, Model *model);
    bool (* fragmentShaderFn)(v3d bar, uint32_t iface, Model *model, SDL_Color *colorOut);
};

void R_putPixel(v2d worldPoint, Uint32 color);
TransformMatrix genViewportTransformMatrix(int x, int y, int w, int h);
TransformMatrix lookAt(v3d eye, v3d center, v3d up);
void R_drawTriangleTextured(v4d *vertices, Model *model, uint32_t iface);

extern v3d gouradVaryingIntensity;
v4d gouradVertexSolidShader(uint32_t iface, uint32_t nthvert, Model *model);
bool gouradFragmentSolidShader(v3d bar, SDL_Color *color);
v4d textureShadingVertexShader(uint32_t iface, uint32_t nthvert, Model *model);
bool textureShadingFragmentShader(v3d bar, uint32_t iface, Model *model, SDL_Color *colorOut);


#endif




