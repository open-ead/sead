#include "thread/seadThread.h"
#include "basis/seadRawPrint.h"
#include "prim/seadScopedLock.h"

namespace sead
{
SEAD_ENUM_IMPL(Thread::State)

SEAD_SINGLETON_DISPOSER_IMPL(ThreadMgr)

ThreadMgr::~ThreadMgr()
{
    ScopedLock<CriticalSection> lock(getListCS());

    for (Thread* thread : mList)
        thread->quit(false);

    bool all_done;
    do
    {
        all_done = true;
        for (Thread* thread : mList)
            all_done &= thread->isDone();
        Thread::yield();
    } while (!all_done);

    for (Thread* thread : mList)
        thread->waitDone();

    sInstance = nullptr;
}

void ThreadMgr::initialize(Heap* heap)
{
    initMainThread_(heap);
    SEAD_ASSERT(mMainThread);
}

void ThreadMgr::destroy()
{
    destroyMainThread_();
}

void ThreadMgr::destroyMainThread_()
{
    if (mMainThread)
    {
        delete mMainThread;
        mMainThread = nullptr;
    }
}

bool ThreadMgr::isMainThread() const
{
    return getCurrentThread() == mMainThread;
}
}  // namespace sead
