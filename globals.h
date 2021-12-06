#ifndef globals_h
#define globals_h

#include <SDL2/SDL.h>
#include "v2d.h"
#include "matrix.h"

// Window, Canvas, Viewport
// Conversions etc
extern SDL_Window *gWindow;
extern SDL_Surface *gWindowSurface;
extern Uint32 *gWindowPixels;
extern v2di gCanvasDimensions;
extern v2d gViewport;
extern float gViewportDistanceFromCamera;

extern v3d eye;
extern float gEyeSpeed;
extern v3d center;
extern float centerSpeed;

extern v3d gLightDir;

extern TransformMatrix modelViewMatrix;
extern TransformMatrix projectionMatrix;
extern TransformMatrix viewportMatrix;

#endif