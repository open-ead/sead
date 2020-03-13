#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <sead/seadArenaCafe.h>
#include <sead/seadCriticalSectionCafe.h>
#include <sead/seadHeap.h>
#include <sead/seadPtrArray.h>

namespace sead {

class HeapMgr
{
public:
    HeapMgr();
    virtual ~HeapMgr() { }

    Heap* getCurrentHeap();
    Heap* findContainHeap(const void* ptr) const;

    static HeapMgr sInstance;
    static HeapMgr* sInstancePtr;

    static Arena sDefaultArena;
    static PtrArrayImpl sRootHeaps;
    static PtrArrayImpl sIndependentHeaps;
    static CriticalSection sHeapTreeLockCS;

    u32 _0;
};

} // namespace sead

#endif // SEAD_HEAPMGR_H_
