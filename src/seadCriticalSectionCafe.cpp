#include <sead.h>

namespace sead {

CriticalSection::CriticalSection()
    : IDisposer()
    , mutex()
{
    OSInitMutex(&mutex);
}

void CriticalSection::lock()
{
    OSLockMutex(&mutex);
}

void CriticalSection::unlock()
{
    OSUnlockMutex(&mutex);
}

} // namespace sead
