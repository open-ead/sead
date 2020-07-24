#ifndef SEAD_CRITICAL_SECTION_H_
#define SEAD_CRITICAL_SECTION_H_

#if defined(cafe)
#include <cafe.h>
#elif defined(NNSDK)
#include <nn/os.h>
#endif

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>

namespace sead
{
class CriticalSection : public IDisposer
{
public:
    CriticalSection();
    virtual ~CriticalSection();

    void lock();
    bool tryLock();
    void unlock();

#if defined(cafe)
    OSMutex mCriticalSectionInner;
#elif defined(NNSDK)
    nn::os::MutexType mCriticalSectionInner;
#else
#error "Unknown platform"
#endif
};

}  // namespace sead

#endif  // SEAD_CRITICAL_SECTION_H_
