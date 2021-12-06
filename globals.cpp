#include <SDL2/SDL.h>
#include "v2d.h"
#include "v3d.h"
#include "matrix.h"

SDL_Window *gWindow;
SDL_Surface *gWindowSurface;
Uint32 *gWindowPixels;
v2di gCanvasDimensions = {800,800};
v2d gViewport;
float gViewportDistanceFromCamera = 1.0f;

v3d eye(0,0,3);
float gEyeSpeed = 0.08;
v3d center(0.5,0,0);
float centerSpeed = 0.08;

v3d gLightDir = {0,0,1};

TransformMatrix modelViewMatrix;
TransformMatrix projectionMatrix;
TransformMatrix viewportMatrix;
