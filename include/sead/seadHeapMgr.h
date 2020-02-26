#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <sead/seadArena.h>
#include <sead/seadCriticalSection.h>
#include <sead/seadHeap.h>
#include <sead/seadPtrArray.h>

namespace sead {

class HeapMgr
{
public:
    HeapMgr();
    virtual ~HeapMgr() { }

    Heap* getCurrentHeap();
    Heap* findContainHeap(void*);

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
