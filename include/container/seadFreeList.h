#pragma once

#include <new>

#include "basis/seadRawPrint.h"
#include "basis/seadTypes.h"

namespace sead
{
class FreeList
{
public:
    void setWork(void* work, s32 elem_size, s32 num);
    void reset();

    void* alloc();
    void free(void* ptr);

    void* getFree() const { return mFree; }
    void* work() const { return mWork; }

    static const size_t cPtrSize = sizeof(void*);

private:
    struct Node
    {
        Node* nextFree;
    };

    Node* mFree = nullptr;
    void* mWork = nullptr;
};

inline void FreeList::setWork(void* work, s32 elem_size, s32 num)
{
    SEAD_ASSERT(work);
    SEAD_ASSERT(elem_size > 0 && elem_size % cPtrSize == 0);
    SEAD_ASSERT(num > 0);

    const s32 nodeSize = elem_size / cPtrSize;
    FreeList::Node* nodes = reinterpret_cast<FreeList::Node*>(work);

    mFree = &nodes[0];

    // Create the linked list.
    for (s32 i = 0; i < num - 1; ++i)
        nodes[i * nodeSize].nextFree = &nodes[(i + 1) * nodeSize];

    // TODO: Check why casting is necessary
    nodes[(s32)((u32)(num - 1) * nodeSize)].nextFree = nullptr;

    mWork = work;
}

inline void FreeList::reset()
{
    mFree = nullptr;
    mWork = nullptr;
}

inline void* FreeList::alloc()
{
    if (!mFree)
        return nullptr;

    void* ptr = mFree;
    mFree = mFree->nextFree;
    return ptr;
}

inline void FreeList::free(void* ptr)
{
    mFree = new (ptr) Node{mFree};
}
}  // namespace sead
