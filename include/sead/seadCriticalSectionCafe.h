#ifndef SEAD_CRITICAL_SECTION_H_
#define SEAD_CRITICAL_SECTION_H_

#include <cafe.h>
#include <sead/seadDisposer.h>

namespace sead {

class CriticalSection : public IDisposer
{
public:
    CriticalSection();
    virtual ~CriticalSection() { }

    void lock();
    void unlock();

    OSMutex mutex;
};

} // namespace sead

#endif // SEAD_CRITICAL_SECTION_H_
