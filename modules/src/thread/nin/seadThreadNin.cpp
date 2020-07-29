#include "thread/seadThread.h"

namespace sead
{
ThreadMgr::ThreadMgr() = default;

u32 ThreadMgr::getCurrentThreadID_()
{
    return u32(uintptr_t(nn::os::GetCurrentThread()));
}
}  // namespace sead
