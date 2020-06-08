#include <sead/seadHeapMgr.h>

#define NUM_ROOT_HEAPS_MAX         4
#define NUM_INDEPENDENT_HEAPS_MAX  4

sead::Heap* rootHeaps[NUM_ROOT_HEAPS_MAX];
sead::Heap* independentHeaps[NUM_INDEPENDENT_HEAPS_MAX];

namespace sead {

HeapMgr* HeapMgr::sInstancePtr = NULL;

HeapMgr HeapMgr::sInstance;
Arena HeapMgr::sDefaultArena;
PtrArrayImpl HeapMgr::sRootHeaps(NUM_ROOT_HEAPS_MAX, rootHeaps);
PtrArrayImpl HeapMgr::sIndependentHeaps(NUM_INDEPENDENT_HEAPS_MAX, independentHeaps);
CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::HeapMgr()
    : mAllocFailedCallback(NULL)
{
}

Heap*
HeapMgr::findContainHeap(const void* ptr) const
{
    Heap* containHeap;
    Heap** heapsEnd;
    Heap** heapsPtr;

    sHeapTreeLockCS.lock();

    heapsEnd = reinterpret_cast<Heap**>(&sRootHeaps.mPtrs[sRootHeaps.mPtrNum]);
    heapsPtr = reinterpret_cast<Heap**>(&sRootHeaps.mPtrs[0]);

    for (; heapsPtr != heapsEnd; heapsPtr++)
    {
        containHeap = (*heapsPtr)->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    heapsEnd = reinterpret_cast<Heap**>(&sIndependentHeaps.mPtrs[sIndependentHeaps.mPtrNum]);
    heapsPtr = reinterpret_cast<Heap**>(&sIndependentHeaps.mPtrs[0]);

    for (; heapsPtr != heapsEnd; heapsPtr++)
    {
        containHeap = (*heapsPtr)->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    sHeapTreeLockCS.unlock();
    return NULL;
}

} // namespace sead
