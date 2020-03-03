#include <sead.h>

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
    : _0(0)
{
}

Heap*
HeapMgr::findContainHeap(const void* ptr) const
{
    Heap* containHeap;
    Heap** heapsEnd;
    Heap** heapsPtr;

    sHeapTreeLockCS.lock();

    heapsEnd = reinterpret_cast<Heap**>(reinterpret_cast<size_t>(sRootHeaps.mPtrs) + sRootHeaps.mSize * 4);
    heapsPtr = static_cast<Heap**>(sRootHeaps.mPtrs);

    for (; heapsPtr != heapsEnd; heapsPtr++)
    {
        containHeap = (*heapsPtr)->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    heapsEnd = reinterpret_cast<Heap**>(reinterpret_cast<size_t>(sIndependentHeaps.mPtrs) + sIndependentHeaps.mSize * 4);
    heapsPtr = static_cast<Heap**>(sIndependentHeaps.mPtrs);

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
