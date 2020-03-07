#include <sead.h>

namespace sead {

void Heap::appendDisposer_(IDisposer* disposer)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* mutex_ = NULL;
    if (flags & 1)
    {
        mutex_ = &mutex;
        mutex_->lock();
    }

    disposers.mStartEnd.insertFront_(disposers.getNodeFromOffset(disposer));
    disposers.mCount += 1;

    if (mutex_ != NULL)
        mutex_->unlock();
}

void Heap::removeDisposer_(IDisposer* disposer)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* mutex_ = NULL;
    if (flags & 1)
    {
        mutex_ = &mutex;
        mutex_->lock();
    }

    (disposers.getNodeFromOffset(disposer))->erase_();
    disposers.mCount -= 1;

    if (mutex_ != NULL)
        mutex_->unlock();
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
    CriticalSection* mutex_ = NULL;
    if (flags & 1)
    {
        mutex_ = &mutex;
        mutex_->lock();
    }

    u32 offset = children.mOffset;
    containHeap = reinterpret_cast<Heap*>(reinterpret_cast<size_t>(children.mStartEnd.mNext) - offset);

    while (containHeap != children.getFromOffsetR<Heap*>(&children))
    {
        if (containHeap->isInclude(ptr))
        {
            containHeap = containHeap->findContainHeap_(ptr);
            if (mutex_ != NULL)
                mutex_->unlock();

            HeapMgr::sHeapTreeLockCS.unlock();
            return containHeap;
        }

        containHeap = reinterpret_cast<Heap*>(*reinterpret_cast<size_t*>(static_cast<void*>(&containHeap->listNode) + offset) - offset);
    }

    if (mutex_ != NULL)
        mutex_->unlock();

    HeapMgr::sHeapTreeLockCS.unlock();
    return this;
}

} // namespace sead
