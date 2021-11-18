#include <stdio.h>
#include "v3d.h"
#include "model.h"
#include <SDL2/SDL.h>
#include "stb_ds.h"
#include <inttypes.h>

Model modelInit(const char *filename){
    Model model;
    model.da_vertices = NULL;
    model.da_faces = NULL;

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
        else if(!strncmp(line, "f ", 2)){
            Uint32 *face = malloc(sizeof(float)*3);
            char *valBeginPtr = line + 2;
            Uint32 lineIdx = 2;
            for(int i = 0; i < 3; i++){
                char *endPtr;
                face[i] = strtol(&line[lineIdx], &endPtr, 10);
                face[i]--;
                lineIdx = (uintptr_t)endPtr - (uintptr_t)line;
                while(line[lineIdx] != ' ' || lineIdx == lineSize-1){
                    lineIdx++;
                }
                lineIdx++;
            }
            stbds_arrpush(model.da_faces, face);
        }
        i++;
    }
    return model;
}

