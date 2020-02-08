#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

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

    u32 _0;
};

} // namespace sead

#endif // SEAD_HEAPMGR_H_
