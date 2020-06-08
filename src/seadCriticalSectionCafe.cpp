#include <sead.h>

namespace sead
{
CriticalSection::CriticalSection() : IDisposer(), mCriticalSectionInner()
{
    OSInitMutex(&mCriticalSectionInner);
}

void CriticalSection::lock()
{
    OSLockMutex(&mCriticalSectionInner);
}

void CriticalSection::unlock()
{
    OSUnlockMutex(&mCriticalSectionInner);
}

}  // namespace sead
