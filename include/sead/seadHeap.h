#ifndef SEAD_HEAP_H_
#define SEAD_HEAP_H_

#include <stddef.h>

#include <sead/seadCriticalSectionCafe.h>
#include <sead/seadDisposer.h>
#include <sead/seadOffsetList.h>
#include <sead/seadRuntimeTypeInfo.h>
#include <sead/seadSafeString.h>

namespace sead { namespace hostio {

class Context;

} // namespace sead::hostio

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
    virtual void* resizeFront(void*, size_t) = 0;
    virtual void* resizeBack(void*, size_t) = 0;
    virtual void freeAll() = 0;
    virtual u32 getStartAddress() const = 0;
    virtual u32 getEndAddress() const = 0;
    virtual u32 getSize() const = 0;
    virtual u32 getFreeSize() const = 0;
    virtual u32 getMaxAllocatableSize(int) const = 0;
    virtual bool isInclude(const void*) const = 0;
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;

    virtual void dump() const
    {
    }

    virtual void genInformation_(hostio::Context*)
    {
    }

    void appendDisposer_(IDisposer* disposer);
    void removeDisposer_(IDisposer* disposer);
    Heap* findContainHeap_(const void* ptr);

    inline void* alloc(size_t size, s32 alignment)
    {
        return tryAlloc(size, alignment);
    }

    SafeString name;
    u32 addr;
    u32 size;
    Heap* parent;
    OffsetList children;
    u32 _34;
    u32 _38;
    OffsetList disposers;
    u32 direction;
    CriticalSection mutex;
    u32 flags;
};

} // namespace sead

#endif // SEAD_HEAP_H_
