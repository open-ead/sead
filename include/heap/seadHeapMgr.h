#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <container/seadPtrArray.h>
#include <heap/seadArena.h>
#include <heap/seadHeap.h>
#include <thread/seadCriticalSection.h>

namespace sead
{
class HeapMgr
{
public:
    HeapMgr();
    virtual ~HeapMgr() {}

    Heap* getCurrentHeap();
    Heap* findContainHeap(const void* ptr) const;

    static HeapMgr* instance() { return sInstancePtr; }

    // TODO: these should be private
    static HeapMgr sInstance;
    static HeapMgr* sInstancePtr;

    using RootHeaps = FixedPtrArray<Heap, 4>;
    using IndependentHeaps = FixedPtrArray<Heap, 4>;

    static Arena sDefaultArena;
    static RootHeaps sRootHeaps;
    static IndependentHeaps sIndependentHeaps;
    static CriticalSection sHeapTreeLockCS;

    void*
        mAllocFailedCallback;  // IAllocFailedCallback* = IDelegate1<const AllocFailedCallbackArg*>*
};

}  // namespace sead

#endif  // SEAD_HEAPMGR_H_
