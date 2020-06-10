#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace sead
{
HeapMgr* HeapMgr::sInstancePtr = NULL;

HeapMgr HeapMgr::sInstance;
Arena HeapMgr::sDefaultArena;
HeapMgr::RootHeaps HeapMgr::sRootHeaps;
HeapMgr::IndependentHeaps HeapMgr::sIndependentHeaps;
CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::HeapMgr() : mAllocFailedCallback(NULL) {}

Heap* HeapMgr::findContainHeap(const void* ptr) const
{
    Heap* containHeap;

    sHeapTreeLockCS.lock();

    for (Heap* heap : sRootHeaps)
    {
        containHeap = heap->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    for (Heap* heap : sIndependentHeaps)
    {
        containHeap = heap->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    sHeapTreeLockCS.unlock();
    return NULL;
}

}  // namespace sead
