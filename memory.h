#ifndef mem_h
#define mem_h

#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_MEMORY_TOTAL 1024


typedef unsigned char byte;



struct MemoryArena{
    byte *backingMemory;
    size_t backingMemorySize;
    uint32_t currentOffset;    
};

extern MemoryArena gMemoryArena;

MemoryArena memoryArenaInit(byte *backingMemory, size_t size);

void *memoryArenaAllocate(MemoryArena *arena, size_t size);

void memoryArenaFree(MemoryArena *arena);

#endif