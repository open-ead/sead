#ifndef SEAD_HEAP_H_
#define SEAD_HEAP_H_

#include <stddef.h>

#include <sead/seadCriticalSection.h>
#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>
#include <sead/seadRuntimeTypeInfo.h>
#include <sead/seadSafeString.h>

namespace sead {

class Heap : public IDisposer
{
public:
    enum HeapDirection
    {
        DirectionHead = 0,
        DirectionTail = 1
    };

    Heap(const SafeString& name, Heap* parent, void* address, u32 size, HeapDirection direction, bool);
    virtual ~Heap() { }

    SEAD_RTTI_BASE(Heap)

    virtual void destroy() = 0;
    virtual u32 adjust() = 0;
    virtual void* tryAlloc(size_t size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;

    // ...

    void appendDisposer_(IDisposer* disposer);
    void removeDisposer_(IDisposer* disposer);
    Heap* findContainHeap_(const void*);

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
