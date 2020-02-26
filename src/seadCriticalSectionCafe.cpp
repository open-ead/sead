#include <sead.h>
#include <ppc_edge.h>

namespace sead {

CriticalSection::CriticalSection()
    : IDisposer()
{
    __memzero(&mutex, sizeof(mutex));
    OSInitMutex(&mutex);
}

} // namespace sead
