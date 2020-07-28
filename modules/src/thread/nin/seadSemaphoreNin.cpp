#include "basis/seadRawPrint.h"
#include "thread/seadSemaphore.h"

namespace sead
{
Semaphore::Semaphore() = default;

Semaphore::Semaphore(s32 initial_count) : Semaphore()
{
    initialize(initial_count);
}

Semaphore::Semaphore(s32 initial_count, s32 max_count) : Semaphore()
{
    initialize(initial_count, max_count);
}

Semaphore::Semaphore(Heap* heap) : Semaphore(heap, HeapNullOption::UseSpecifiedOrContainHeap) {}

Semaphore::Semaphore(Heap* heap, s32 initial_count) : Semaphore(heap)
{
    initialize(initial_count);
}

Semaphore::Semaphore(Heap* heap, s32 initial_count, s32 max_count) : Semaphore(heap)
{
    initialize(initial_count, max_count);
}

Semaphore::Semaphore(Heap* heap, IDisposer::HeapNullOption heap_null_option)
    : IDisposer(heap, heap_null_option)
{
}

Semaphore::Semaphore(Heap* heap, IDisposer::HeapNullOption heap_null_option, s32 initial_count)
    : Semaphore(heap, heap_null_option)
{
    initialize(initial_count);
}

Semaphore::Semaphore(Heap* heap, IDisposer::HeapNullOption heap_null_option, s32 initial_count,
                     s32 max_count)
    : Semaphore(heap, heap_null_option)
{
    initialize(initial_count, max_count);
}

Semaphore::~Semaphore()
{
    nn::os::FinalizeSemaphore(&mSemaphoreInner);
    mInitialized = false;
}

void Semaphore::initialize(s32 initial_count, s32 max_count)
{
    SEAD_ASSERT_MSG(!mInitialized, "Semaphore is already initialized.");
    nn::os::InitializeSemaphore(&mSemaphoreInner, initial_count, max_count);
    mInitialized = true;
}

void Semaphore::lock()
{
    SEAD_ASSERT_MSG(mInitialized, "Semaphore is not initialized.");
    nn::os::AcquireSemaphore(&mSemaphoreInner);
}

bool Semaphore::tryLock()
{
    SEAD_ASSERT_MSG(mInitialized, "Semaphore is not initialized.");
    return nn::os::TryAcquireSemaphore(&mSemaphoreInner);
}

void Semaphore::unlock()
{
    SEAD_ASSERT_MSG(mInitialized, "Semaphore is not initialized.");
    nn::os::ReleaseSemaphore(&mSemaphoreInner);
}
}  // namespace sead
