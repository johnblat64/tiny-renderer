#define V3D_IMPLEMENTATION
#include "v3d.h"
#define V2D_IMPLEMENTATION
#include "v2d.h"
#define V4D_IMPLEMENTATION
#include "v4d.h"
#include "globals.h"
#include <inttypes.h>
#include "util.h"
#include "matrix.h"
#include "r.h"
#include "model.h"

const uint32_t depth = 255;

v3d gouradVaryingIntensity(0,0,0);


//ZBUFFER
float *g_zBuffer; 


TransformMatrix genViewportTransformMatrix(int x, int y, int w, int h){
    TransformMatrix tm = genIdTransformMatrix();
    tm._[0][3] = x+w/2.0f;
    tm._[1][3] = y+h/2.0f;
    tm._[2][3] = depth/2.0f;

    tm._[0][0] = w/2.0f;
    tm._[1][1] = h/2.0f;
    tm._[2][2] = depth/2.0f;

    return tm; 
}


TransformMatrix lookAt(v3d eye, v3d center, v3d up){
    v3d z = v3d_normal(eye - center);
    v3d x = v3d_normal(v3d_cross(up, z));
    v3d y = v3d_normal(v3d_cross(z, x));
    // TransformMatrix Minv = genIdTransformMatrix();
    // TransformMatrix Tr = genIdTransformMatrix();
    TransformMatrix modelView = genIdTransformMatrix();
    for(int i = 0; i < 3; i++){
        modelView._[0][i] = x[i];
        modelView._[1][i] = y[i];
        modelView._[2][i] = z[i];
        modelView._[i][3] = -center[i];
    }

    return modelView;
}



inline void R_putPixel(v2d canvasPoint, Uint32 color){
    // if(canvasPoint.x < -(gCanvasDimensions.x >> 1) || canvasPoint.x > (gCanvasDimensions.x >> 1)){
    //     return;
    // }
    // if(canvasPoint.y < -(gCanvasDimensions.y >> 1) || canvasPoint.y > (gCanvasDimensions.y >> 1)){
    //     return;
    // }

    // v2di screenPoint = canvasPointToScreenPoint(canvasPoint);
    v2di screenPoint = v2di((int)canvasPoint.x, gCanvasDimensions.y - (int)canvasPoint.y);
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

static v3d baycentric(v2d *pts, v2d P){

    v2d v0 = pts[1] - pts[0];
    v2d v1 = pts[2] - pts[0];
    v2d v2 = P - pts[0];

    float d00 = v0 * v0;
    float d01 = v0 * v1;
    float d11 = v1 * v1;
    float d20 = v2 * v0;
    float d21 = v2 * v1;
    float v, w, u;
    if(d00 * d11 - d01 * d01 == 0){
         v = 0;
    }
    else
        v = (d11 * d20 - d01 * d21) / (d00 * d11 - d01 * d01);
    if(d00 * d11 - d01 * d01 == 0)
        w = 0;
    else
        w = (d00 * d21 - d01 * d20) / (d00 * d11 - d01 * d01);
    
    
    u = 1.0f - v - w;

    return v3d(v, w, u); 

}


static v2d getP(v3d bcScreen, v2d *pts){
    float v = bcScreen.x;
    float w = bcScreen.y;
    float u = bcScreen.z;

    v2d P = (pts[0] * u) + (pts[1] * v) + (pts[2] * w);
    return P;


}


void R_drawTriangleTextured(v4d *vertices, SDL_Color *texture, Uint32 texture_w, v2di *textureCoords){
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
            // SDL_Color li_color = {
            //     (Uint8)(lightIntensity*color.r),
            //     (Uint8)(lightIntensity*color.g),
            //     (Uint8)(lightIntensity*color.b),
            //     255
            // };
            bool discard = gouradFragmentShader(bcScreen, &color);
            if(!discard){
                Uint32 color32 = SDL_MapRGB(gWindowSurface
                ->format, color.r, color.g, color.b);

                if(g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] < P.z){
                    g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] = P.z;
                    R_putPixel(
                        (v2d){P.x, P.y},
                        color32
                    );
                }
            }
            // else {
            //     // printf("not!\n");
            // }
            // if((int)P.x % 8 == 0){
            // SDL_UpdateWindowSurface(gWindow);

            // }
            
                
        }
    }
}

v4d gouradVertexShader(uint32_t iface, uint32_t nthvert, Model *model){
    if(nthvert > 3){
        assert(0 && "nthvert out of bounds for gourad vertex shader\n");
    }

    Face nFace = model->da_normalFaces
    [iface];
    v3d n = model->da_normals[nFace[nthvert]];
    // n.y *= -1;
    // n.x *= -1;
    float nDotLightDir = n*gLightDir;
    // nDotLightDir *= -1;
    gouradVaryingIntensity[nthvert] = max(0.0f, n*gLightDir);
    Face vFace = model->da_faces[iface];
    v3d vert = model->da_vertices[vFace[nthvert]];
    v4d vert4d(vert.x, vert.y, vert.z, 1.0f);
    v4d result = viewportMatrix * projectionMatrix * modelViewMatrix * vert4d;
    // v4d resultv4d(
    //     resultM._[0][0], 
    //     resultM._[1][0],
    //     resultM._[2][0], 
    //     resultM._[3][0]
    // );
    return result;
}

bool gouradFragmentShader(v3d bar, SDL_Color *color){
    swapValues(bar.x, bar.z, float);
    swapValues(bar.z, bar.y, float);
    float intensity = gouradVaryingIntensity*bar;
    color->r = 255 * intensity;
    color->g = 255 * intensity;
    color->b = 255 * intensity;

    return false;
}