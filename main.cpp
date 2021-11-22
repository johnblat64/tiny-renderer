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

v3d gLightDir = {0,0,-1};

#define max(a,b)(a > b ? a : b)
#define min(a,b)(a < b ? a : b)


// Window, Canvas, Viewport
// Conversions etc
static SDL_Window *gWindow;
static SDL_Surface *gWindowSurface;
static Uint32 *gWindowPixels;
static v2di gCanvasDimensions = {640,640};
static v2d gViewport;
static float gViewportDistanceFromCamera = 1.0f;

v2di canvasPointToScreenPoint(v2d worldPoint);
void R_putPixel(v2d worldPoint, SDL_Color color);


inline v2di canvasPointToScreenPoint(v2d canvasPoint){
    v2di px = {
        (gCanvasDimensions.x >> 1) + (int)canvasPoint.x,
        (gCanvasDimensions.y >> 1) - (int)canvasPoint.y
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
    v2di screenPoint = {(int)canvasPoint.x, (int)canvasPoint.y};
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
        p0->x,
        p0->y
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
                    (float)y,
                    (float)x
                },
                color
            );
        }
        else {
            R_putPixel(
                (v2d){
                    (float)x,
                    (float)y
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
    if(fabsf(u.z) < 1.0f){
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
        (float)gCanvasDimensions.x,
        (float)gCanvasDimensions.y
    };


    v2d bboxMax = {0, 0};
    v2d clamp = {(float)gCanvasDimensions.x, (float)gCanvasDimensions.y};
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            bboxMin.raw[j] = max(0, min(bboxMin.raw[j], vs[i].raw[j]));
            bboxMax.raw[j] = min(clamp.raw[j], max(bboxMax.raw[j], vs[i].raw[j]));
        }
    }

    v2d P = {0};
  
    for(P.y = bboxMin.y; P.y <= bboxMax.y; P.y++){
        for(P.x = bboxMin.x; P.x <= bboxMax.x; P.x++){
            v3d bcScreen = baycentric(vs, P);
            
            if(bcScreen.x < 0 || bcScreen.y < 0 || bcScreen.z < 0){
                continue;
            }
            
            R_putPixel(
                (v2d){P.x, P.y},
                color
            );
            
                // SDL_Delay(50);
                // SDL_UpdateWindowSurface(gWindow);
            
                
        }
    }
    
}

void R_drawTriangle2(v2d *vs, SDL_Color color){
    if(vs[0].y==vs[1].y && vs[0].y == vs[2].y){
        return;
    }

    if(vs[0].y > vs[1].y){
        swapValues(vs[0], vs[1], v2d);
    }
    if(vs[0].y > vs[2].y){
        swapValues(vs[0], vs[2], v2d);
    }
    if(vs[1].y > vs[2].y){
        swapValues(vs[1], vs[2], v2d);
    }

    int totalHeight = vs[2].y - vs[0].y;

    for(int i = 0; i < totalHeight; i++){
        bool secondHalf = i > vs[1].y - vs[0].y || vs[1].y == vs[0].y;
        int segmentHeight = secondHalf ? vs[2].y - vs[1].y : vs[1].y - vs[0].y;
        float alpha = (float)i/(float)totalHeight;
        float beta = (float)(i-(secondHalf ? vs[1].y - vs[0].y : 0)) / (float)segmentHeight;
        
        v2d A = v2d_add(vs[0], v2d_scale(alpha, v2d_sub(vs[2], vs[0])));
        v2d B = secondHalf ? v2d_add(vs[1], v2d_scale(beta, v2d_sub(vs[2],vs[1]))) : v2d_add(vs[0], v2d_scale( beta, v2d_sub(vs[1], vs[0])));

        if(A.x>B.x){
            swapValues(A, B, v2d);
        }
        for(int j = A.x; j <= B.x; j++){
            R_putPixel(
                (v2d){
                    (float)j,
                    vs[0].y+i
                },
                color
            );
             SDL_Delay(10);
                SDL_UpdateWindowSurface(gWindow);
        }
    }

}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Tiny Renderer", 0, 0, gCanvasDimensions.x, gCanvasDimensions.y, 0);
    gWindowSurface = SDL_GetWindowSurface(gWindow);
    gWindowPixels = (Uint32 *)gWindowSurface->pixels;

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
            v2d screenCoords[3];
            v3d worldCoords[3];
            for(int j = 0; j < 3; j++){
                v3d v = model.da_vertices[face[j]];
                screenCoords[j] = (v2d){
                    (v.x+1)*gCanvasDimensions.x/2.0f,
                    (-v.y+1)*gCanvasDimensions.y/2.0f
                };
                worldCoords[j] = v;
            }
            v3d a = v3d_sub(worldCoords[2], worldCoords[0]);
            v3d b = v3d_sub(worldCoords[1], worldCoords[0]);
            v3d n = v3d_cross(a, b);
            n = v3d_normal(n);

            float lightIntensity = v3d_dot(n, gLightDir);

            if(lightIntensity > 0){
                R_drawTriangle(
                    screenCoords,
                    (SDL_Color){
                        (Uint8)(lightIntensity*255),
                        (Uint8)(lightIntensity*255),
                        (Uint8)(lightIntensity*255),
                        255
                    }
                );
            }
            
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