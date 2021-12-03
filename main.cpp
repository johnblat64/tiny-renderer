#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION
#define V3D_IMPLEMENTATION
#include "v3d.h"
#define V2D_IMPLEMENTATION
#include "v2d.h"
#include "math.h"
#include "model.h"
#include <SDL2/SDL.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA
#include "stb_image.h"
#include "globals.h"
#include "matrix.h"
#include "memory.h"


// Color Constants
const SDL_Color BLUE = {0,0,255,255};
const SDL_Color GREEN = {0,255,0,255};
const SDL_Color RED = {255,0,0,255};
const SDL_Color CYAN = {0,255,255,255};
const SDL_Color YELLOW = {255,255,0,255};
const SDL_Color PURPLE = {255,0,255,255};
const SDL_Color WHITE = {255,255,255,255};
const SDL_Color DARK_MODE = {25,25,25,255};

v3d camera(0,0,3);

v3d gLightDir = {0,0,-1};

#define max(a,b)(a > b ? a : b)
#define min(a,b)(a < b ? a : b)



//ZBUFFER
float *g_zBuffer; 

v2di canvasPointToScreenPoint(v2d worldPoint);
void R_putPixel(v2d worldPoint, Uint32 color);


inline v2di canvasPointToScreenPoint(v2d canvasPoint){
    v2di px = v2di(
        ((int)gCanvasDimensions.x >> 1) + (int)canvasPoint.x,
        ((int)gCanvasDimensions.y >> 1) - (int)canvasPoint.y
    );
    return px;
}

inline void R_putPixel(v2d canvasPoint, Uint32 color){
    // if(canvasPoint.x < -(gCanvasDimensions.x >> 1) || canvasPoint.x > (gCanvasDimensions.x >> 1)){
    //     return;
    // }
    // if(canvasPoint.y < -(gCanvasDimensions.y >> 1) || canvasPoint.y > (gCanvasDimensions.y >> 1)){
    //     return;
    // }

    // v2di screenPoint = canvasPointToScreenPoint(canvasPoint);
    v2di screenPoint = v2di((int)canvasPoint.x, (int)canvasPoint.y);
    unsigned int idx = (screenPoint.y * gWindowSurface->w) + screenPoint.x;
    // uint32_t colorU32 = SDL_MapRGBA(
    //     gWindowSurface->format,
    //     color.r,
    //     color.g,
    //     color.b,
    //     color.a
    // );

    gWindowPixels[idx] = color;
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
    Uint32 color32 = SDL_MapRGB(gWindowSurface->format, color.r, color.g, color.b);
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
                color32
            );
        }
        else {
            R_putPixel(
                (v2d){
                    (float)x,
                    (float)y
                },
                color32
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
    // v3d pt0 = {
    //     pts[2].x-pts[0].x,
    //     pts[1].x-pts[0].x,
    //     pts[0].x-P.x
    // };
    // v3d pt1 = {
    //     pts[2].y-pts[0].y,
    //     pts[1].y-pts[0].y,
    //     pts[0].y-P.y
    // };
    // v3d u = v3d_cross(pt0, pt1);
    // if(fabsf(u.z) < 1.0f){
    //     return (v3d){-1, 1, 1};
    // }
    // return (v3d){
    //     1.0f-(u.x+u.y)/u.z,
    //     u.y/u.z,
    //     u.x/u.z
    // };
    // v2d pts2 = pts;
    // pts
    v2d v0 = pts[1] - pts[0];
    v2d v1 = pts[2] - pts[0];
    v2d v2 = P - pts[0];

    float d00 = v0 * v0;
    float d01 = v0 * v1;
    float d11 = v1 * v1;
    float d20 = v2 * v0;
    float d21 = v2 * v1;
    
    float v = (d11 * d20 - d01 * d21) / (d00 * d11 - d01 * d01);
    float w = (d00 * d21 - d01 * d20) / (d00 * d11 - d01 * d01);
    float u = 1.0f - v - w;

    return v3d(v, w, u); 

}

v2d getP(v3d bcScreen, v2d *pts){
    float v = bcScreen.x;
    float w = bcScreen.y;
    float u = bcScreen.z;

    v2d P = (pts[0] * u) + (pts[1] * v) + (pts[2] * w);
    return P;


}

void R_drawTriangleTextured(v3d *vertices, SDL_Color *texture, float lightIntensity, Uint32 texture_w, v2di *textureCoords){
    v2d tcs_f[3];
    for(int i = 0; i < 3; i++){
        tcs_f[i] = v2d(textureCoords[i].x, textureCoords[i].y);
    }
    
    //sort
    v2d bboxMin = {
        (float)gCanvasDimensions.x,
        (float)gCanvasDimensions.y
    };

    v2d v2ds[3];
    for(int i = 0; i < 3; i++){
        v2ds[i].x = vertices[i].x;
        v2ds[i].y = vertices[i].y;
    }

    v2d bboxMax = {0, 0};
    v2d clamp = {(float)gCanvasDimensions.x, (float)gCanvasDimensions.y};
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            bboxMin[j] = max(0, min(bboxMin[j], vertices[i][j]));
            bboxMax[j] = min(clamp[j], max(bboxMax[j], vertices[i][j]));
        }
    }

    v3d P = {0, 0, 0};
  
    for(P.y = (int)bboxMin.y; P.y <= (int)bboxMax.y; P.y++){
        for(P.x = (int)bboxMin.x; P.x <= (int)bboxMax.x; P.x++){
            v3d bcScreen = baycentric(v2ds, v2d(P.x, P.y));
            v2d tc = getP(bcScreen, tcs_f);

            if(bcScreen.x < 0.0f || bcScreen.y < 0.0f || bcScreen.z < 0.0f){
                continue;
            }
            P.z = 0;
            for(int i = 0; i < 3; i++){
                P.z += vertices[i].z * bcScreen[i];
            }

            Uint32 idx = (int)tc.x + (int)tc.y * texture_w;
            SDL_Color color = texture[idx];
            SDL_Color li_color = {
                (Uint8)(lightIntensity*color.r),
                (Uint8)(lightIntensity*color.g),
                (Uint8)(lightIntensity*color.b),
                255
            };
            Uint32 color32 = SDL_MapRGB(gWindowSurface
            ->format, li_color.r, li_color.g, li_color.b);

            if(g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] <= P.z){
                g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] = P.z;
                R_putPixel(
                    (v2d){P.x, P.y},
                    color32
                );
            }
            else {
                printf("not!\n");
            }
            // if((int)P.x % 8 == 0){
            // SDL_UpdateWindowSurface(gWindow);

            // }
            
                
        }
    }
}


void R_drawTriangle(v3d *vs, SDL_Color color){
    Uint32 color32 = SDL_MapRGB(gWindowSurface->format, color.r, color.g, color.b);
    //sort
    v2d bboxMin = {
        (float)gCanvasDimensions.x,
        (float)gCanvasDimensions.y
    };

    v2d v2ds[3];
    for(int i = 0; i < 3; i++){
        v2ds[i].x = vs[i].x;
        v2ds[i].y = vs[i].y;
    }

    v2d bboxMax = {0, 0};
    v2d clamp = {(float)gCanvasDimensions.x, (float)gCanvasDimensions.y};
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            bboxMin[j] = max(0, min(bboxMin[j], vs[i][j]));
            bboxMax[j] = min(clamp[j], max(bboxMax[j], vs[i][j]));
        }
    }

    v3d P = {0, 0, 0};
  
    for(P.y = (int)bboxMin.y; P.y <= (int)bboxMax.y; P.y++){
        for(P.x = (int)bboxMin.x; P.x <= (int)bboxMax.x; P.x++){
            v3d bcScreen = baycentric(v2ds, v2d(P.x, P.y));
            
            if(bcScreen.x < 0.0f || bcScreen.y < 0.0f || bcScreen.z < 0.0f){
                continue;
            }
            P.z = 0;
            for(int i = 0; i < 3; i++){
                P.z += vs[i].z * bcScreen[i];
            }
            if(g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] <= P.z){
                g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] = P.z;
                R_putPixel(
                    (v2d){P.x, P.y},
                    color32
                );
            }
            else {
                printf("not!\n");
            }
            
                // SDL_Delay(50);
                // SDL_UpdateWindowSurface(gWindow);
            
                
        }
    }
    
}

// void R_drawTriangle2(v2d *vs, SDL_Color color){
//     if(vs[0].y==vs[1].y && vs[0].y == vs[2].y){
//         return;
//     }

//     if(vs[0].y > vs[1].y){
//         swapValues(vs[0], vs[1], v2d);
//     }
//     if(vs[0].y > vs[2].y){
//         swapValues(vs[0], vs[2], v2d);
//     }
//     if(vs[1].y > vs[2].y){
//         swapValues(vs[1], vs[2], v2d);
//     }

//     int totalHeight = vs[2].y - vs[0].y;

//     for(int i = 0; i < totalHeight; i++){
//         bool secondHalf = i > vs[1].y - vs[0].y || vs[1].y == vs[0].y;
//         int segmentHeight = secondHalf ? vs[2].y - vs[1].y : vs[1].y - vs[0].y;
//         float alpha = (float)i/(float)totalHeight;
//         float beta = (float)(i-(secondHalf ? vs[1].y - vs[0].y : 0)) / (float)segmentHeight;
        
//         v2d A = v2d_add(vs[0], v2d_scale(alpha, v2d_sub(vs[2], vs[0])));
//         v2d B = secondHalf ? v2d_add(vs[1], v2d_scale(beta, v2d_sub(vs[2],vs[1]))) : v2d_add(vs[0], v2d_scale( beta, v2d_sub(vs[1], vs[0])));

//         if(A.x>B.x){
//             swapValues(A, B, v2d);
//         }
//         for(int j = A.x; j <= B.x; j++){
//             R_putPixel(
//                 (v2d){
//                     (float)j,
//                     vs[0].y+i
//                 },
//                 color
//             );
//              SDL_Delay(10);
//                 SDL_UpdateWindowSurface(gWindow);
//         }
//     }

// }

const uint32_t depth = 255;

// Matrix v2m(v3d v){
//     Matrix m(4, 1);
//     m[0][0] = v.x;
//     m[1][0] = v.y;
//     m[2][0] = v.z;
//     m[3][0] = 1.0f;

//     return m;
// }

// v3d m2v(Matrix m){
//     v3d v(
//         m[0][0]/m[3][0],
//         m[1][0]/m[3][0],
//         m[2][0]/m[3][0]
//     );
//     return v;
// }

v3d m2v(TransformMatrix tm){
    v3d v(
        tm.m[0][0]/tm.m[3][0],
        tm.m[1][0]/tm.m[3][0],
        tm.m[2][0]/tm.m[3][0]
    );
    return v;
}

TransformMatrix genViewportTransformMatrix(int x, int y, int w, int h){
    TransformMatrix tm = genIdTransformMatrix(4);
    tm.m[0][3] = x+w/2.0f;
    tm.m[1][3] = y+h/2.0f;
    tm.m[2][3] = depth/2.0f;

    tm.m[0][0] = w/2.0f;
    tm.m[1][1] = h/2.0f;
    tm.m[2][2] = depth/2.0f;

    return tm; 
}

int main(){
    byte backingMemory[DEFAULT_MEMORY_TOTAL];
    //mem
    gMemoryArena = memoryArenaInit(backingMemory, DEFAULT_MEMORY_TOTAL);

    //other
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Tiny Renderer", 0, 0, gCanvasDimensions.x, gCanvasDimensions.y, 0);
    gWindowSurface = SDL_GetWindowSurface(gWindow);
    gWindowPixels = (Uint32 *)gWindowSurface->pixels;

    Model model = modelInit("african-head.obj");
    // for(int i = 0; i < stbds_arrlen(model.da_vertices); i++){
    //     model.da_vertices[i].y-=0.99;
    // }

    float zBuffer[gCanvasDimensions.x * gCanvasDimensions.y]; 
    g_zBuffer = zBuffer;
    // memset(g_zBuffer, 0, sizeof(int) * gCanvasDimensions.x * gCanvasDimensions.y);
    for(int i = 0; i < gCanvasDimensions.x * gCanvasDimensions.y; i++){
        g_zBuffer[i] =  -FLT_MAX;
    }

    

    SDL_bool quit = SDL_FALSE;
    SDL_Event event;
    while(!quit){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = SDL_TRUE;
            }
        }

        memoryArenaFree(&gMemoryArena);

        TransformMatrix projectionMatrix = genIdTransformMatrix(4);
        TransformMatrix viewportMatrix = genViewportTransformMatrix(
            gCanvasDimensions.x/8,
            gCanvasDimensions.y/8,
            gCanvasDimensions.x*3/4, 
            gCanvasDimensions.y*3/4
        );
        projectionMatrix.m[3][2] = -1.0f/camera.z;

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

        // uint32_t nFaces = stbds_arrlen(model.da_faces);
        for(int i = 0; i < stbds_arrlen(model.da_faces); i++){

            // for(int i = 0; i < gCanvasDimensions.x * gCanvasDimensions.y; i++){
            //     g_zBuffer[i] =  FLT_MIN;
            // }      
            if(i > 2492){
                printf("what?\n");
            }
            Face vFace = model.da_faces[i];
            Face tFace = model.da_textureFaces[i];

            v3d screenCoords[3];
            v3d xscreenCoords[3];

            v3d worldCoords[3];
            v2di textureCoords[3];
            
            for(int j = 0; j < 3; j++){
                v3d v = model.da_vertices[vFace[j]];
                v4d v1 = v4d(v.x, -v.y, v.z, 1.0f);
                v2d vtc = model.da_textureCoordinates[tFace[j]];
                xscreenCoords[j] = (v3d){
                    (v.x+1)*gCanvasDimensions.x/2.0f,
                    (-v.y+1)*gCanvasDimensions.y/2.0f,
                    v.z // keep the z for a z buffer
                };
                screenCoords[j] = m2v(viewportMatrix * projectionMatrix * v1);
                // screenCoords[j].y *= -1;

                worldCoords[j] = v;

                textureCoords[j] = v2di( 
                    model.da_textureCoordinates[tFace[j]].x,
                    model.da_textureCoordinates[tFace[j]].y
                 );
                v2d vtc2 = v2d(
                    (vtc.x+1)*model.texture_w/2.0f,
                    (vtc.y+1)*model.texture_h/2.0f
                ); 
                textureCoords[j].x = (vtc.x)*model.texture_w;
                textureCoords[j].y = (vtc.y)*(model.texture_h);
                textureCoords[j].y = model.texture_h - textureCoords[j].y;  
            }
            
            v3d a = worldCoords[2] - worldCoords[0];
            v3d b = worldCoords[1] - worldCoords[0];
            v3d n = v3d_cross(a, b);
            n = v3d_normal(n);

            float lightIntensity = v3d_dot(n, gLightDir);
          
            if(lightIntensity > 0){
                // R_draw
                // R_drawTriangle(
                //     screenCoords,
                //     (SDL_Color){
                //         (Uint8)(lightIntensity*255),
                //         (Uint8)(lightIntensity*255),
                //         (Uint8)(lightIntensity*255),
                //         255
                //     }
                // );
                R_drawTriangleTextured(
                    screenCoords,
                    model.da_texture,
                    lightIntensity,
                    model.texture_w,
                    textureCoords
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