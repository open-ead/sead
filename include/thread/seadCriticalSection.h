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
class Heap;

class CriticalSection
#if not SEAD_CRITICALSECTION_PURE
    : public IDisposer
#endif
{
public:
    CriticalSection();
    explicit CriticalSection(Heap* disposer_heap);
#if not SEAD_CRITICALSECTION_PURE
    CriticalSection(Heap* disposer_heap, HeapNullOption heap_null_option);
#endif
    ~CriticalSection() SEAD_CRITIALSECTION_OVERRIDE_TOKEN;

    CriticalSection(const CriticalSection&) = delete;
    CriticalSection& operator=(const CriticalSection&) = delete;

    void lock();
    bool tryLock();
    void unlock();

    // For compatibility with the standard Lockable concept.
    bool try_lock() { return tryLock(); }

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
