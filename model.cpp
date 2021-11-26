#include <stdio.h>
#include "v3d.h"
#include "model.h"
#include <SDL2/SDL.h>
#include "stb_ds.h"
#include <inttypes.h>
#define STBI_ONLY_TGA
#include "stb_image.h"
#include "globals.h"

Model modelInit(const char *filename){
    Model model;
    model.da_vertices = NULL;
    model.da_faces = NULL;
    model.da_textureCoordinates = NULL;
    model.da_textureFaces = NULL;
    model.da_texture = NULL;

    SDL_RWops *modelReadContext = SDL_RWFromFile(filename, "r");
    size_t numBytes = modelReadContext->size(modelReadContext);
    char content[numBytes + 1];
    SDL_RWread(modelReadContext, content, sizeof(char), numBytes);
    content[numBytes] = '\0';

    char *line;
    size_t lineSize = 0;
    Uint32 lineStart = 0;
    Uint32 lineEnd = 0;

    Uint32 i = 0;
    
    while(content[i] != '\0'){
        lineStart = i;
        while(content[i] != '\n'){
            i++;
        }
        lineEnd = i;
        lineSize = lineEnd - lineStart;
        line = (char *)(content + lineStart);
        // memcpy(line, content + lineStart, lineSize);
        if(!strncmp(line, "v ", 2)){
            v3d v;
            char *valBeginPtr = line + 2;
            
            char *endPtr;
            v.x = strtof(valBeginPtr, &endPtr);
            valBeginPtr = endPtr + 1;
            v.y = strtof(valBeginPtr, &endPtr);
            valBeginPtr = endPtr + 1;
            v.z = strtof(valBeginPtr, &endPtr); 
            
            stbds_arrpush(model.da_vertices, v);
        }
        else if(!strncmp(line, "vt", 2)){
            v2d v;
            char *valBeginPtr = line + 2;
            char *endPtr;
            v.x = strtof(valBeginPtr, &endPtr);
            valBeginPtr = endPtr + 1;
            v.y = strtof(valBeginPtr, &endPtr);

            stbds_arrpush(model.da_textureCoordinates, v);
        }
        else if(!strncmp(line, "f ", 2)){
            Face vFace = {0};
            Face tFace = {0};
            char *valBeginPtr = line + 2;
            Uint32 lineIdx = 2;
            char *endPtr;
            vFace.i0 = strtol(&line[lineIdx], &endPtr, 10);
            vFace.i0--;
            
            lineIdx = (uintptr_t)endPtr- (uintptr_t)line+1;
            tFace.i0 = strtol(&line[lineIdx], &endPtr, 10);
            tFace.i0--;

            while(line[lineIdx] != ' ' || lineIdx == lineSize-1){
                lineIdx++;
            }
            lineIdx++;
            vFace.i1 = strtol(&line[lineIdx], &endPtr, 10);
            vFace.i1--;

            lineIdx = (uintptr_t)endPtr- (uintptr_t)line+1;
            tFace.i1 = strtol(&line[lineIdx], &endPtr, 10);
            tFace.i1--;

            while(line[lineIdx] != ' ' || lineIdx == lineSize-1){
                lineIdx++;
            }
            lineIdx++;
            vFace.i2 = strtol(&line[lineIdx], &endPtr, 10);
            vFace.i2--;

            lineIdx = (uintptr_t)endPtr- (uintptr_t)line+1;
            tFace.i2 = strtol(&line[lineIdx], &endPtr, 10);
            tFace.i2--;

            while(line[lineIdx] != ' ' || lineIdx == lineSize-1){
                lineIdx++;
            }
            lineIdx++;

            stbds_arrpush(model.da_faces, vFace);
            stbds_arrpush(model.da_textureFaces, tFace);
        }
        i++;
    }
    int x, y, channelsInFile;
    // channelsInFile = 4;
    stbi_uc *imageBytes = stbi_load(
        "african_head_diffuse.tga",
        &x,
        &y,
        &channelsInFile,
        3
    );

    for(int i = 0; i < x * y * 3; i+=3){
        SDL_Color color = {
            imageBytes[i],
            imageBytes[i+1],
            imageBytes[i+2]
        };
        // Uint32 color32 = SDL_MapRGB(
        //     gWindowSurface->format,
        //     color.r,
        //     color.g,
        //     color.b
        // );
        stbds_arrpush(model.da_texture, color);
    }

    model.texture_w = x;
    model.texture_h = y;
    
    stbi_image_free(imageBytes);
    return model;
}

