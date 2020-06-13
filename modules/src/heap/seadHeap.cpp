#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

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

    mDisposerList.pushBack(disposer);

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
    if (!isInclude(ptr))
        return nullptr;

    for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
    {
        if (it->isInclude(ptr))
            return it->findContainHeap_(ptr);
    }

    return this;
}

}  // namespace sead
