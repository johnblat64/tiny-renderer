#ifndef globals_h
#define globals_h

#include <SDL2/SDL.h>
#include "v2d.h"
// Window, Canvas, Viewport
// Conversions etc
extern SDL_Window *gWindow;
extern SDL_Surface *gWindowSurface;
extern Uint32 *gWindowPixels;
extern v2di gCanvasDimensions;
extern v2d gViewport;
extern float gViewportDistanceFromCamera;

#endif