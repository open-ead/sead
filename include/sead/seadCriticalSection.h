#ifndef SEAD_CRITICAL_SECTION_H_
#define SEAD_CRITICAL_SECTION_H_

#if defined(cafe)
#include <cafe.h>
#elif defined(SWITCH)
#include <nn/os.h>
#endif

#include <sead/seadDisposer.h>
#include <sead/types.h>

namespace sead {

class CriticalSection : public IDisposer
{
public:
    CriticalSection();
    virtual ~CriticalSection() { }

    void lock();
    void unlock();

#if defined(cafe)
    OSMutex mCriticalSectionInner;
#elif defined(SWITCH)
    nn::os::MutexType mCriticalSectionInner;
#else
    #error "Unknown platform"
#endif
};

} // namespace sead

#endif // SEAD_CRITICAL_SECTION_H_
