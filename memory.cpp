
#include "memory.h"

MemoryArena gMemoryArena;

MemoryArena memoryArenaInit(byte *backingMemory, size_t size){
    MemoryArena m;
    m.backingMemory = backingMemory;
    memset(m.backingMemory, 0, size);
    m.backingMemorySize = size;
    m.currentOffset = 0;
    return m;
}

void *memoryArenaAllocate(MemoryArena *arena, size_t size){
    void *ptr;

    assert(arena->currentOffset + size < arena->backingMemorySize);

    ptr = arena->backingMemory + arena->currentOffset;
    arena->currentOffset += size;

    return ptr;
}

void memoryArenaFree(MemoryArena *arena){
    arena->currentOffset = 0;
    memset(arena->backingMemory, 0, arena->backingMemorySize);
}