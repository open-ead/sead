#include <sead/seadHeap.h>
#include <sead/seadHeapMgr.h>

namespace sead {

void Heap::appendDisposer_(IDisposer* disposer)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = NULL;
    if (mFlag.mBits & 1)
    {
        cs = &mCS;
        cs->lock();
    }

    mDisposerList.mStartEnd.insertFront_(mDisposerList.getNodeFromOffset(disposer));
    mDisposerList.mCount += 1;

    if (cs != NULL)
        cs->unlock();
}

void Heap::removeDisposer_(IDisposer* disposer)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = NULL;
    if (mFlag.mBits & 1)
    {
        cs = &mCS;
        cs->lock();
    }

    (mDisposerList.getNodeFromOffset(disposer))->erase_();
    mDisposerList.mCount -= 1;

    if (cs != NULL)
        cs->unlock();
}

Heap*
Heap::findContainHeap_(const void* ptr)
{
    Heap* containHeap;

    HeapMgr::sHeapTreeLockCS.lock();

    if (!isInclude(ptr))
    {
        HeapMgr::sHeapTreeLockCS.unlock();
        return NULL;
    }

    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = NULL;
    if (mFlag.mBits & 1)
    {
        cs = &mCS;
        cs->lock();
    }

    u32 offset = mChildren.mOffset;
    containHeap = reinterpret_cast<Heap*>(reinterpret_cast<size_t>(mChildren.mStartEnd.mNext) - offset);

    while (containHeap != mChildren.getFromOffsetR<Heap*>(&mChildren))
    {
        if (containHeap->isInclude(ptr))
        {
            containHeap = containHeap->findContainHeap_(ptr);
            if (cs != NULL)
                cs->unlock();

            HeapMgr::sHeapTreeLockCS.unlock();
            return containHeap;
        }

        containHeap = reinterpret_cast<Heap*>(*reinterpret_cast<size_t*>(static_cast<void*>(&static_cast<IDisposer*>(containHeap)->mListNode) + offset) - offset);
    }

    if (cs != NULL)
        cs->unlock();

    HeapMgr::sHeapTreeLockCS.unlock();
    return this;
}

} // namespace sead
