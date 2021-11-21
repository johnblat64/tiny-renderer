#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#define V3D_IMPLEMENTATION
#include "v3d.h"
#define V2D_IMPLEMENTATION
#include "v2d.h"
#include "math.h"
#include "model.h"
#include <SDL2/SDL.h>


// Color Constants
const SDL_Color BLUE = {0,0,255,255};
const SDL_Color GREEN = {0,255,0,255};
const SDL_Color RED = {255,0,0,255};
const SDL_Color CYAN = {0,255,255,255};
const SDL_Color YELLOW = {255,255,0,255};
const SDL_Color PURPLE = {255,0,255,255};
const SDL_Color WHITE = {255,255,255,255};
const SDL_Color DARK_MODE = {25,25,25,255};


#define max(a,b)(a > b ? a : b)
#define min(a,b)(a < b ? a : b)


// Window, Canvas, Viewport
// Conversions etc
static SDL_Window *gWindow;
static SDL_Surface *gWindowSurface;
static uint32_t *gWindowPixels;
static v2di gCanvasDimensions = {640,640};
static v2d gViewport;
static float gViewportDistanceFromCamera = 1.0f;

v2di canvasPointToScreenPoint(v2d worldPoint);
void R_putPixel(v2d worldPoint, SDL_Color color);


inline v2di canvasPointToScreenPoint(v2d canvasPoint){
    v2di px = {
        .x = (gCanvasDimensions.x >> 1) + canvasPoint.x,
        .y = (gCanvasDimensions.y >> 1) - canvasPoint.y
    };
    return px;
}

inline void R_putPixel(v2d canvasPoint, SDL_Color color){
    // if(canvasPoint.x < -(gCanvasDimensions.x >> 1) || canvasPoint.x > (gCanvasDimensions.x >> 1)){
    //     return;
    // }
    // if(canvasPoint.y < -(gCanvasDimensions.y >> 1) || canvasPoint.y > (gCanvasDimensions.y >> 1)){
    //     return;
    // }

    // v2di screenPoint = canvasPointToScreenPoint(canvasPoint);
    v2di screenPoint = {canvasPoint.x, canvasPoint.y};
    unsigned int idx = (screenPoint.y * gWindowSurface->w) + screenPoint.x;
    uint32_t colorU32 = SDL_MapRGBA(
        gWindowSurface->format,
        color.r,
        color.g,
        color.b,
        color.a
    );

    gWindowPixels[idx] = colorU32;
}


v2d worldPointToViewportPoint(v3d worldPoint){
    float d = gViewportDistanceFromCamera;

    v2d P_prime = {
        worldPoint.x * d / worldPoint.z,
        worldPoint.y * d / worldPoint.z
    };
    return P_prime;
}


v2d viewportPointToCanvasPoint(v2d viewportPoint){
    v2d canvasPoint = {
        viewportPoint.x * gCanvasDimensions.x / gViewport.x,
        viewportPoint.y * gCanvasDimensions.y / gViewport.y
    };
    return canvasPoint;
}

v2d projectWorldVertex(v3d worldVertex){
    v2d canvasPoint;
    float d = gViewportDistanceFromCamera;
    v2d viewportPoint = {
        worldVertex.x * d / worldVertex.z,
        worldVertex.y * d / worldVertex.z
    };
    canvasPoint = viewportPointToCanvasPoint(viewportPoint);
    return canvasPoint;
}


void swapPoints(v2d *p0, v2d *p1){
    v2d temp = {
        .x = p0->x,
        .y = p0->y
    };

    p0->x = p1->x;
    p0->y = p1->y;

    p1->x = temp.x;
    p1->y = temp.y;
}

#define swapValues(a, b, T){ \
    T temp = a; \
    a = b; \
    b = temp; \
}

typedef struct Line2d {
    v2d v0, v1;
} Line2d;

typedef struct Line3d {
    v2d v0, v1, v2;
} Line3d;




void R_drawLineGradient(SDL_Renderer *renderer, Line2d line, SDL_Color color, float h0, float h1){

}


void R_drawLineSolid(Line2d line, SDL_Color color){
    SDL_bool steep = SDL_FALSE;

    if(fabsf(line.v0.x - line.v1.x) < fabsf(line.v0.y - line.v1.y)){
        swapValues(line.v0.x, line.v0.y, float);
        swapValues(line.v1.x, line.v1.y, float);
        steep = SDL_TRUE;
    }
    if(line.v0.x > line.v1.x){
        swapValues(line.v0.x, line.v1.x, float);
        swapValues(line.v0.y, line.v1.y, float);
    }
    int dx = line.v1.x - line.v0.x;
    int dy = line.v1.y - line.v0.y;
    int derror2 = abs(dy)*2;
    int error2 = 0;
    int y = line.v0.y;
    for(int x = line.v0.x; x <= line.v1.x; x++){
        if(steep){
            R_putPixel(
                (v2d){
                    .x = y,
                    .y = x
                },
                color
            );
        }
        else {
            R_putPixel(
                (v2d){
                    .x = x,
                    .y = y
                },
                color
            );
        }
        error2 += derror2;
        if(error2 > dx) {
            y += (line.v1.y > line.v0.y ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}



v3d baycentric(v2d *pts, v2d P){
    v3d pt0 = {
        pts[2].x-pts[0].x,
        pts[1].x-pts[0].x,
        pts[0].x-P.x
    };
    v3d pt1 = {
        pts[2].y-pts[0].y,
        pts[1].y-pts[0].y,
        pts[0].y-P.y
    };
    v3d u = v3d_cross(pt0, pt1);
    if(fabsf(u.z) < 1){
        return (v3d){-1, 1, 1};
    }
    return (v3d){
        1.0f-(u.x+u.y)/u.z,
        u.y/u.z,
        u.x/u.z
    };
}


void R_drawTriangle(v2d *vs, SDL_Color color){
    //sort
    v2d bboxMin = {
        gCanvasDimensions.x-1,
        gCanvasDimensions.y-1
    };


    v2d bboxMax = {0, 0};
    v2d clamp = {gCanvasDimensions.x-1, gCanvasDimensions.y-1};
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            bboxMin.raw[j] = max(0, min(bboxMin.raw[j], vs[i].raw[j]));
            bboxMax.raw[j] = min(clamp.y, max(bboxMax.raw[j], vs[i].raw[j]));
        }
    }

    v2d P = {0};
    
    for(P.x = bboxMin.x; P.x <= bboxMax.x; P.x++){
        for(P.y = bboxMin.y; P.y <= bboxMax.y; P.y++){
            v3d bcScreen = baycentric(vs, P);
            
            if(bcScreen.x < 0 || bcScreen.y < 0 || bcScreen.z < 0){
                continue;
            }
            
            R_putPixel(
                (v2d){P.x, P.y},
                color
            );
        }
    }
    
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Tiny Renderer", 0, 0, gCanvasDimensions.x, gCanvasDimensions.y, 0);
    gWindowSurface = SDL_GetWindowSurface(gWindow);
    gWindowPixels = gWindowSurface->pixels;

    Model model = modelInit("african-head.obj");

    SDL_bool quit = SDL_FALSE;
    SDL_Event event;
    while(!quit){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = SDL_TRUE;
            }
        }
        Line2d line0 = {
            (v2d){50,50},
            (v2d){100,100}
        };

        Uint32 bgColor = SDL_MapRGB(
            gWindowSurface->format,
            DARK_MODE.r, DARK_MODE.g, DARK_MODE.b
        );

        SDL_FillRect(gWindowSurface, NULL, bgColor);

        // R_drawLineSolid(line0, GREEN);

        for(int i = 0; i < stbds_arrlen(model.da_faces); i++){
            Uint32 *face = (Uint32 * )&model.da_faces[i];
            v2d screen_coords[3];
            for(int j = 0; j < 3; j++){
                v3d worldCoords = model.da_vertices[face[j]];
                screen_coords[j] = (v2d){
                    (worldCoords.x+1)*gCanvasDimensions.x/2.0f,
                    (-worldCoords.y+1)*gCanvasDimensions.y/2.0f
                };
                
                // v3d v0 = model.da_vertices[face[j]];
                // v3d v1 = model.da_vertices[face[(j+1)%3]];
                // Line2d line = {
                //     (v2d){(v0.x+1.0f)*gCanvasDimensions.x*0.5, (-v0.y+1.0f)*gCanvasDimensions.y*0.5},
                //     (v2d){(v1.x+1.0f)*gCanvasDimensions.x*0.5, (-v1.y+1.0f)*gCanvasDimensions.y*0.5}
                // };
                // R_drawLineSolid(line, GREEN);
            }
            R_drawTriangle(
                screen_coords,
                (SDL_Color){
                    rand()%255,
                    rand()%255,
                    rand()%255,
                    255
                }
            );
        }
        // v2d vs[3] = {
        //     (v2d){10,10},
        //     (v2d){100,30},
        //     (v2d){190,160}
        // };
        // R_drawTriangle(vs, GREEN);
        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay(50);
    }

    exit(EXIT_SUCCESS);
}