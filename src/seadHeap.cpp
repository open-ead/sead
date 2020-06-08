#include <sead/seadHeap.h>
#include <sead/seadHeapMgr.h>

namespace sead
{
void Heap::appendDisposer_(IDisposer* disposer)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = NULL;
    if (mFlag.mBits & 1)
    {
        cs = &mCS;
        cs->lock();
    }

    mDisposerList.insertFront(disposer);

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

    mDisposerList.erase(disposer);

    if (cs != NULL)
        cs->unlock();
}

Heap* Heap::findContainHeap_(const void* ptr)
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

    for (Heap& child : mChildren) {
        containHeap = &child;
        if (containHeap->isInclude(ptr))
        {
            containHeap = containHeap->findContainHeap_(ptr);
            if (cs != NULL)
                cs->unlock();

            HeapMgr::sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    if (cs != NULL)
        cs->unlock();

    HeapMgr::sHeapTreeLockCS.unlock();
    return this;
}

}  // namespace sead
