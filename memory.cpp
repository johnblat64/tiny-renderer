
#include "memory.h"
#include <stdio.h>

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
    if(arena->currentOffset + size > arena->backingMemorySize){
        fprintf(stderr, "desired  total size %lu. actual total size: %lu\n", arena->currentOffset + size, arena->backingMemorySize );
        assert(0 &&"gotta fail");
    }

    ptr = arena->backingMemory + arena->currentOffset;
    arena->currentOffset += size;

    return ptr;
}

void memoryArenaFree(MemoryArena *arena){
    arena->currentOffset = 0;
    memset(arena->backingMemory, 0, arena->backingMemorySize);
}

MemoryStack gMemoryStack;

MemoryStack memoryStackInit(byte *backingMemory, size_t size){
    gMemoryStack.backingMemory = backingMemory;
    gMemoryStack.backingMemorySize = size;
    gMemoryStack.currentOffset = 0;
    gMemoryStack.previousOffset = 0;
    return gMemoryStack;
}

void *memoryStackAllocate(MemoryStack *stack, size_t size){
    uintptr_t currentAddress, nextAddress;
    size_t paddingSize;
    MemoryStackAllocationHeader *header;

    currentAddress = (uintptr_t)stack->backingMemory + (uintptr_t)stack->currentOffset;
    paddingSize = sizeof(MemoryStackAllocationHeader);

    if(stack->currentOffset + paddingSize + size > stack->backingMemorySize){
        fprintf(stderr, "desired  total size %lu. actual total size: %lu\n", stack->currentOffset + paddingSize + size, stack->backingMemorySize );
        assert(0 &&"gotta fail");
    }

    stack->previousOffset = stack->currentOffset;;
    stack->currentOffset += paddingSize;

    nextAddress = currentAddress + (uintptr_t)paddingSize;
    header = (MemoryStackAllocationHeader *)(nextAddress - sizeof(MemoryStackAllocationHeader));
    header->padding = (uint8_t)paddingSize;
    header->previousOffset = stack->previousOffset;

    stack->currentOffset += size;

    return memset((void *)nextAddress, 0, size);

}