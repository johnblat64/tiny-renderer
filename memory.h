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

struct MemoryStack{
    byte *backingMemory;
    size_t backingMemorySize;
    uint32_t previousOffset;
    uint32_t currentOffset;
};


struct MemoryStackAllocationHeader{
    size_t previousOffset;
    uint32_t padding;
};

extern MemoryStack gMemoryStack;

MemoryStack memoryStackInit(byte *backingMemory, size_t size);

void *memoryStackAllocate(MemoryStack *stack, size_t size);

/**
 * @brief frees the memory at this pointer and everything above it, if any
 * 
 * @param stack pointer to the memory stack
 * @param ptr pointer to the memory allocated within the memory stack
 */
void memoryStackFree(MemoryStack *stack, void *ptr);

#endif