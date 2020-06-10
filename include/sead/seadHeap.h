#ifndef SEAD_HEAP_H_
#define SEAD_HEAP_H_

#include <stddef.h>

#include <sead/seadBitFlag.h>
#include <sead/seadCriticalSection.h>
#include <sead/seadDisposer.h>
#include <sead/seadNamable.h>
#include <sead/seadOffsetList.h>
#include <sead/seadRuntimeTypeInfo.h>
#include <sead/seadSafeString.hpp>
#include <sead/types.h>

namespace sead
{
class WriteStream;

namespace hostio
{
class Context;
class PropertyEvent;
}  // namespace hostio

class Heap : public IDisposer, public INamable
{
public:
    enum HeapDirection
    {
        cHeapDirection_Forward = 1,
        cHeapDirection_Reverse = -1
    };

    Heap(const SafeString& name, Heap* parent, void* address, size_t size, HeapDirection direction,
         bool);
    virtual ~Heap() {}

    SEAD_RTTI_BASE(Heap)

    virtual void destroy() = 0;
    virtual size_t adjust() = 0;
    virtual void* tryAlloc(size_t size, s32 alignment) = 0;
    virtual void free(void* ptr) = 0;
    virtual void* resizeFront(void*, size_t) = 0;
    virtual void* resizeBack(void*, size_t) = 0;
    virtual void* tryRealloc(void* ptr, size_t size, s32 alignment);
    virtual void freeAll() = 0;
    virtual uintptr_t getStartAddress() const = 0;
    virtual uintptr_t getEndAddress() const = 0;
    virtual size_t getSize() const = 0;
    virtual size_t getFreeSize() const = 0;
    virtual size_t getMaxAllocatableSize(int alignment) const = 0;
    virtual bool isInclude(const void*) const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isFreeable() const = 0;
    virtual bool isResizable() const = 0;
    virtual bool isAdjustable() const = 0;

    virtual void dump() const {}
    virtual void dumpYAML(WriteStream& stream, int) const;

    virtual void listenPropertyEvent(const hostio::PropertyEvent* event);
    virtual void genMessage(hostio::Context*);
    virtual void genInformation_(hostio::Context*) {}
    virtual void makeMetaString_(BufferedSafeString*);

    virtual void pushBackChild_(Heap* child);

    void appendDisposer_(IDisposer* disposer);
    void removeDisposer_(IDisposer* disposer);
    Heap* findContainHeap_(const void* ptr);

    inline void* alloc(size_t size, s32 alignment) { return tryAlloc(size, alignment); }

    using HeapList = OffsetList<Heap>;
    using DisposerList = OffsetList<IDisposer>;

    // TODO: this appears to be a hostio::Node. In Labo, there is even a hostio::Reflexible here.
    void* hostioNode;
    void* mStart;
    size_t mSize;
    Heap* mParent;
    HeapList mChildren;
    ListNode mListNode;
    DisposerList mDisposerList;
    HeapDirection mDirection;
    CriticalSection mCS;
    BitFlag32 mFlag;
};

}  // namespace sead

#endif  // SEAD_HEAP_H_
