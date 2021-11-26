#include <SDL2/SDL.h>
#include "v2d.h"

SDL_Window *gWindow;
SDL_Surface *gWindowSurface;
Uint32 *gWindowPixels;
v2di gCanvasDimensions = {640,640};
v2d gViewport;
float gViewportDistanceFromCamera = 1.0f;