#include <sead.h>

sead::Heap* rootHeaps[4];
sead::Heap* independentHeaps[4];

namespace sead {

HeapMgr* HeapMgr::sInstancePtr = NULL;

HeapMgr HeapMgr::sInstance;
Arena HeapMgr::sDefaultArena;
PtrArrayImpl HeapMgr::sRootHeaps(sizeof(Heap*), rootHeaps);
PtrArrayImpl HeapMgr::sIndependentHeaps(sizeof(Heap*), independentHeaps);
CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::HeapMgr()
    : _0(0)
{
}

} // namespace sead
