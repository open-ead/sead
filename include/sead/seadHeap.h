#ifndef SEAD_HEAP_H_
#define SEAD_HEAP_H_

#include <sead/seadCriticalSection.h>
#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>

namespace sead {

class Heap : public IDisposer
{
public:
    enum HeapDirection { };

    Heap(SafeString& name, Heap* parent, void* address, u32 size, HeapDirection direction, bool);
    virtual ~Heap() { }
    // ...

    void appendDisposer_(IDisposer* disposer);
    void removeDisposer_(IDisposer* disposer);
    Heap* findContainHeap_(void*);

    SafeString name;
    u32 addr;
    u32 size;
    Heap* parent;
    ListImpl children;
    u32 _34;
    u32 _38;
    ListImpl disposers;
    u32 direction;
    CriticalSection mutex;
    u32 flags;
};

} // namespace sead

#endif // SEAD_HEAP_H_
