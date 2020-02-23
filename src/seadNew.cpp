#include <stddef.h>
#include <sead.h>

#include <cafe.h>

/*
    This file should actually be implemented
    by the application using sead.
    I am implementing it myself for convenience.
*/

void* operator new(size_t size)
{
    if (sead::HeapMgr::sInstancePtr != NULL)
    {
        sead::Heap* heap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        if (heap == NULL)
            return NULL;

        return heap->tryAlloc(size, 4);
    }

    return (*MEMAllocFromDefaultHeap)(size);
}

void* operator new[](size_t size)
{
    if (sead::HeapMgr::sInstancePtr != NULL)
    {
        sead::Heap* heap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        if (heap == NULL)
            return NULL;

        return heap->tryAlloc(size, 4);
    }

    return (*MEMAllocFromDefaultHeap)(size);
}

void operator delete(void* ptr)
{
    if (ptr != NULL)
    {
        if (sead::HeapMgr::sInstancePtr != NULL)
        {
            sead::Heap* heap = sead::HeapMgr::sInstancePtr->findContainHeap(ptr);
            if (heap == NULL)
                return;

            return heap->free(ptr);
        }

        return (*MEMFreeToDefaultHeap)(ptr);
    }
}

void operator delete[](void* ptr)
{
    if (ptr != NULL)
    {
        if (sead::HeapMgr::sInstancePtr != NULL)
        {
            sead::Heap* heap = sead::HeapMgr::sInstancePtr->findContainHeap(ptr);
            if (heap == NULL)
                return;

            return heap->free(ptr);
        }

        return (*MEMFreeToDefaultHeap)(ptr);
    }
}

void* operator new(size_t size, sead::Heap* heap, s32 alignment)
{
    if (heap == NULL)
    {
        heap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        if (heap == NULL)
            return NULL;
    }

    return heap->tryAlloc(size, alignment);
}

void* operator new[](size_t size, sead::Heap* heap, s32 alignment)
{
    if (heap == NULL)
    {
        heap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        if (heap == NULL)
            return NULL;
    }

    return heap->tryAlloc(size, alignment);
}
