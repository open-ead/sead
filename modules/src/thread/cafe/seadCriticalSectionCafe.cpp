#include <thread/seadCriticalSection.h>

namespace sead
{
CriticalSection::CriticalSection() : IDisposer(), mCriticalSectionInner()
{
    OSInitMutex(&mCriticalSectionInner);
}

CriticalSection::~CriticalSection()
{
}

void CriticalSection::lock()
{
    OSLockMutex(&mCriticalSectionInner);
}

bool CriticalSection::tryLock()
{
    return OSTryLockMutex(&mCriticalSectionInner);
}

void CriticalSection::unlock()
{
    OSUnlockMutex(&mCriticalSectionInner);
}

}  // namespace sead
