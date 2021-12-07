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

v3d eye(1,1,5);
float gEyeSpeed = 0.08;
v3d center(0,0,0);
float centerSpeed = 0.08;

v3d gLightDir = {1,1,1};

TransformMatrix modelViewMatrix;
TransformMatrix projectionMatrix;
TransformMatrix viewportMatrix;
