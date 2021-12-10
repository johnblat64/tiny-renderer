#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION
#define V3D_IMPLEMENTATION
#include "v3d.h"
#define V2D_IMPLEMENTATION
#include "v2d.h"
#define V4D_IMPLEMENTATION
#include "v4d.h"
#include "math.h"
#include "model.h"
#include <SDL2/SDL.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA
#include "stb_image.h"
#include "globals.h"
#include "matrix.h"
#include "r.h"


// Color Constants
const SDL_Color BLUE = {0,0,255,255};
const SDL_Color GREEN = {0,255,0,255};
const SDL_Color RED = {255,0,0,255};
const SDL_Color CYAN = {0,255,255,255};
const SDL_Color YELLOW = {255,255,0,255};
const SDL_Color PURPLE = {255,0,255,255};
const SDL_Color WHITE = {255,255,255,255};
const SDL_Color DARK_MODE = {25,25,25,255};







void clearZBuffer(float *zBuffer, size_t size){
    Uint32 timeBefore = SDL_GetTicks();
    for(int i = 0; i < size; i++){
        zBuffer[i] = -FLT_MAX;
    }
    Uint32 timeAfter = SDL_GetTicks();
    printf(" time to clear zBuffer: %d\n", timeAfter-timeBefore);
}


int main(){
    // byte backingMemory[DEFAULT_MEMORY_TOTAL];
    //mem
    // gMemoryArena = memoryArenaInit(backingMemory, DEFAULT_MEMORY_TOTAL);

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

        clearZBuffer(g_zBuffer, gCanvasDimensions.x*gCanvasDimensions.y);

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = SDL_TRUE;
            }
            
        }
        //input
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        if(keyState[SDL_SCANCODE_W]){
            eye.z-=gEyeSpeed;
            center.z-=gEyeSpeed;

        }
        if(keyState[SDL_SCANCODE_S]){
            eye.z+=gEyeSpeed;
            center.z+=gEyeSpeed;

        }
        if(keyState[SDL_SCANCODE_A]){
            eye.x-=gEyeSpeed;
            center.x-=gEyeSpeed;

        }
        if(keyState[SDL_SCANCODE_D]){
            eye.x+=gEyeSpeed;
            center.x+=gEyeSpeed;

        }
        if(keyState[SDL_SCANCODE_LEFT]){
            center.x -= centerSpeed;
            // center.z -= centerSpeed;
        }
        if(keyState[SDL_SCANCODE_RIGHT]){
            center.x += centerSpeed;
            // center.z += centerSpeed;

        }


        // memoryArenaFree(&gMemoryArena);

        modelViewMatrix = lookAt(eye, center, v3d(0,1,0));
        projectionMatrix = genIdTransformMatrix();
        viewportMatrix = genViewportTransformMatrix(
            gCanvasDimensions.x/8,
            gCanvasDimensions.y/8,
            gCanvasDimensions.x*3/4, 
            gCanvasDimensions.y*3/4
        );
        // viewportMatrix = genViewportTransformMatrix(
        //     200,
        //     200,
        //     gCanvasDimensions.x/2, 
        //     gCanvasDimensions.y/2
        // );
        projectionMatrix._[3][2] = -1.0f/v3d_magnitude(eye-center);
// projectionMatrix._[3][2] = -1.0f/(eye-center).z;
        gLightDir = v3d_unit(gLightDir);

        Uint32 bgColor = SDL_MapRGB(
            gWindowSurface->format,
            DARK_MODE.r, DARK_MODE.g, DARK_MODE.b
        );

        SDL_FillRect(gWindowSurface, NULL, bgColor);

        // R_drawLineSolid(line0, GREEN);

        // uint32_t nFaces = stbds_arrlen(model.da_faces);


        Uint32 timeBefore = SDL_GetTicks();

        for(int i = 0; i < stbds_arrlen(model.da_faces); i++){

            // for(int i = 0; i < gCanvasDimensions.x * gCanvasDimensions.y; i++){
            //     g_zBuffer[i] =  FLT_MIN;
            // }      
            if(i > 2492){
                printf("what?\n");
            }
            Face vFace = model.da_faces[i];
            Face tFace = model.da_textureFaces[i];

            v4d screenCoords[3];

            v3d worldCoords[3];

            for(int j = 0; j < 3; j++){
                screenCoords[j] = textureShadingVertexShader(i, j, &model);
                
            }

            
            R_drawTriangleTextured(
                screenCoords,
                &model,
                i
            );

            
            // for(int j = 0; j < 3; j++){
            //     // v3d v = model.da_vertices[vFace[j]];
            //     // v4d v1 = v4d(v.x, -v.y, v.z, 1.0f);
            //     // v2d vtc = model.da_textureCoordinates[tFace[j]];
            //     // xscreenCoords[j] = (v3d){
            //     //     (v.x+1)*gCanvasDimensions.x/2.0f,
            //     //     (-v.y+1)*gCanvasDimensions.y/2.0f,
            //     //     v.z // keep the z for a z buffer
            //     // };
            //     // screenCoords[j] = m2v(viewportMatrix * projectionMatrix * modelViewMatrix * v1);
            //     // screenCoords[j].y *= -1;


            //     // worldCoords[j] = v;

            //     // textureCoords[j] = v2di( 
            //     //     model.da_textureCoordinates[tFace[j]].x,
            //     //     model.da_textureCoordinates[tFace[j]].y
            //     //  );
            //     // v2d vtc2 = v2d(
            //     //     (vtc.x+1)*model.texture_w/2.0f,
            //     //     (vtc.y+1)*model.texture_h/2.0f
            //     // ); 
            //     // textureCoords[j].x = (vtc.x)*model.texture_w;
            //     // textureCoords[j].y = (vtc.y)*(model.texture_h);
            //     // textureCoords[j].y = model.texture_h - textureCoords[j].y;  
            // }
            
            // v3d a = worldCoords[2] - worldCoords[0];
            // v3d b = worldCoords[1] - worldCoords[0];
            // v3d n = v3d_cross(a, b);
            // n = v3d_normal(n);

            // float lightIntensity = v3d_dot(n, gLightDir);
          
            // if(lightIntensity > 0){
            //     // R_draw
            //     // R_drawTriangle(
            //     //     screenCoords,
            //     //     (SDL_Color){
            //     //         (Uint8)(lightIntensity*255),
            //     //         (Uint8)(lightIntensity*255),
            //     //         (Uint8)(lightIntensity*255),
            //     //         255
            //     //     }
            //     // );
            //     R_drawTriangleTextured(
            //         screenCoords,
            //         model.da_texture,
            //         lightIntensity,
            //         model.texture_w,
            //         textureCoords
            //     );
            // }
            
        }
        Uint32 timeAfter = SDL_GetTicks();
        printf("time to render scene: %d\n", timeAfter-timeBefore);
        // v2d vs[3] = {
        //     (v2d){10,10},
        //     (v2d){100,30},
        //     (v2d){190,160}
        // };
        // R_drawTriangle(vs, GREEN);
        SDL_UpdateWindowSurface(gWindow);
        // SDL_Delay(1);
    }

    exit(EXIT_SUCCESS);
}