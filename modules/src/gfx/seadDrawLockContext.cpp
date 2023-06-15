#include "gfx/seadDrawLockContext.h"

namespace sead
{
DrawLockContext::DrawLockContext() = default;

void DrawLockContext::initialize(Heap*) {}

void DrawLockContext::lock()
{
    mCriticalSection.lock();
}
void DrawLockContext::unlock()
{
    mCriticalSection.unlock();
}

void DrawLockContext::genMessage(hostio::Context*) {}

}  // namespace sead
