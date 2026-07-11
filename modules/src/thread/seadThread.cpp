#include "thread/seadThread.h"
#include "basis/seadRawPrint.h"
#include "prim/seadBitUtil.h"
#include "prim/seadPtrUtil.h"
#include "prim/seadScopedLock.h"
#include "thread/seadThreadUtil.h"

namespace sead
{
const s32 Thread::cDefaultPriority = 0x10;

bool Thread::sendMessage(MessageQueue::Element msg, MessageQueue::BlockType block_type)
{
    if (msg == MessageQueue::cNullElement)
    {
        SEAD_ASSERT_MSG(false, "Can not send cNullElement(==%ld)", MessageQueue::cNullElement);
        return false;
    }

    if (isDone())
    {
        SEAD_ASSERT_MSG(false, "Thread is done. Reject message: %ld", msg);
        return false;
    }

    if (mQuitMsg == msg)
    {
        SEAD_ASSERT_MSG(false, "use quit()");
        return false;
    }

    return mMessageQueue.push(msg, block_type);
}

MessageQueue::Element Thread::recvMessage(MessageQueue::BlockType block_type)
{
    if (mState == State::cQuitting)
        return 0;
    return mMessageQueue.pop(block_type);
}

void Thread::quit(bool is_jam)
{
    if (isDone())
    {
        SEAD_WARN("Thread is done. Can not quit.");
        return;
    }

    if (mState == State::cRunning)
        mState = State::cQuitting;

    if (is_jam)
        mMessageQueue.jam(mQuitMsg, MessageQueue::BlockType::Blocking);
    else
        mMessageQueue.push(mQuitMsg, MessageQueue::BlockType::Blocking);
}

void Thread::quitAndWaitDoneSingleThread(bool is_jam)
{
    quit(is_jam);
    waitDone();
}

constexpr u32 cStackCanaryMagic = 0x5EAD5CEC;

static bool checkStackMagic(uintptr_t addr)
{
    return BitUtil::bitCastPtr<u32>(reinterpret_cast<const void*>(addr)) == cStackCanaryMagic;
}

s32 Thread::calcStackUsedSizePeak() const
{
#ifdef SEAD_DEBUG
    // FIXME
    return 0;
#else
    return 0;
#endif
}

void Thread::checkStackOverFlow(const char*, s32) const {}

void Thread::checkStackEndCorruption(const char*, s32) const {}

void Thread::checkStackPointerOverFlow(const char*, s32) const {}

void Thread::setStackOverflowExceptionEnable(bool)
{
    SEAD_WARN("This platform cannot set stack overflow exception.");
}

void Thread::run_()
{
    while (true)
    {
#ifdef SEAD_DEBUG
        checkStackOverFlow(nullptr, 0);
#endif

        const MessageQueue::Element msg = mMessageQueue.pop(mBlockType);
        if (msg == mQuitMsg)
            break;

        calc_(msg);
    }
}

void Thread::initStackCheck_() {}

void Thread::initStackCheckWithCurrentStackPointer_() {}

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

void ThreadMgr::waitDoneMultipleThread(Thread* const* threads, s32 num)
{
    bool all_done;
    do
    {
        all_done = true;
        for (s32 i = 0; i < num; ++i)
            all_done &= threads[i]->isDone();
        Thread::yield();
    } while (!all_done);

    for (s32 i = 0; i < num; ++i)
        threads[i]->waitDone();
}

void ThreadMgr::quitAndWaitDoneMultipleThread(Thread** threads, s32 num, bool is_jam)
{
    for (s32 i = 0; i < num; ++i)
        threads[i]->quit(is_jam);

    waitDoneMultipleThread(threads, num);
}

void ThreadMgr::checkCurrentThreadStackOverFlow(const char*, s32) {}

void ThreadMgr::checkCurrentThreadStackEndCorruption(const char*, s32) {}

void ThreadMgr::checkCurrentThreadStackPointerOverFlow(const char*, s32) {}
}  // namespace sead
