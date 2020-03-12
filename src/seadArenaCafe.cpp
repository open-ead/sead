#include <cafe.h>
#include <sead.h>

namespace sead {

Arena::Arena()
    : heap(NULL)
    , size(0)
{
}

Arena::~Arena()
{
}

void* Arena::initialize(size_t size)
{
    MEMHeapHandle handle = MEMGetBaseHeapHandle(MEM_ARENA_2);
    u32 allocSize = MEMGetAllocatableSizeForExpHeap(handle);
    if (size > allocSize)
        size = allocSize;

    this->size = size;
    heap = (*MEMAllocFromDefaultHeapEx)(size, MEM_HEAP_DEFAULT_ALIGNMENT);
    this->heap = heap;

    return heap;
}

} // namespace sead
