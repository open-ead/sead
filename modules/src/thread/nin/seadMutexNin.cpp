#include "thread/seadMutex.h"

namespace sead
{
Mutex::Mutex() : IDisposer()
{
    nn::os::InitializeMutex(&mMutexInner, true, 0);
}

Mutex::Mutex(Heap* disposer_heap)
    : IDisposer(disposer_heap, HeapNullOption::UseSpecifiedOrContainHeap)
{
    nn::os::InitializeMutex(&mMutexInner, true, 0);
}

Mutex::~Mutex()
{
    nn::os::FinalizeMutex(&mMutexInner);
}

void Mutex::lock()
{
    nn::os::LockMutex(&mMutexInner);
}

bool Mutex::tryLock()
{
    return nn::os::TryLockMutex(&mMutexInner);
}

void Mutex::unlock()
{
    nn::os::UnlockMutex(&mMutexInner);
}
}  // namespace sead
