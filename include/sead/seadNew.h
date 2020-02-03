#ifndef SEAD_NEW_H_
#define SEAD_NEW_H_

#include <stddef.h>
#include <sead/seadHeap.h>

void* operator new(size_t size, sead::Heap* heap, s32 alignment);
void operator delete(void* ptr);

void* operator new[](size_t size, sead::Heap* heap, s32 alignment);
void operator delete[](void* ptr);

#endif // SEAD_NEW_H_
