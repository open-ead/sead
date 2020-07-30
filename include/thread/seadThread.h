#pragma once

#ifdef NNSDK
#include <nn/os.h>
#endif

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeapMgr.h>
#include <hostio/seadHostIONode.h>
#include <hostio/seadHostIOReflexible.h>
#include <mc/seadCoreInfo.h>
#include <prim/seadEnum.h>
#include <prim/seadNamable.h>
#include <prim/seadSafeString.hpp>
#include <prim/seadScopedLock.h>
#include <thread/seadMessageQueue.h>
#include <thread/seadThreadLocalStorage.h>
#include <time/seadTickSpan.h>

namespace sead
{
class Heap;
class Thread;

using ThreadList = TList<Thread*>;
using ThreadListNode = TListNode<Thread*>;

class Thread : public IDisposer, public INamable, public hostio::Reflexible
{
public:
    SEAD_ENUM(State, cInitialized, cRunning, cQuitting, cTerminated, cReleased);

    Thread(const SafeString& name, Heap* heap, s32 priority, MessageQueue::BlockType block_type,
           MessageQueue::Element quit_msg, s32 stack_size, s32 message_queue_size);
    ~Thread() override;

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    virtual void destroy();

    virtual bool sendMessage(MessageQueue::Element msg, MessageQueue::BlockType block_type);
    virtual MessageQueue::Element recvMessage(MessageQueue::BlockType block_type);
    virtual const MessageQueue& getMessageQueue() const;

    virtual bool start();
    virtual void quit(bool is_jam);
    virtual void waitDone();
    virtual void quitAndDestroySingleThread(bool is_jam);
    virtual void quitAndWaitDoneSingleThread(bool is_jam);

    virtual void setPriority(s32 prio);
    virtual s32 getPriority() const;
    virtual MessageQueue::BlockType getBlockType() const;
    virtual s32 getStackSize() const;
    virtual s32 calcStackUsedSizePeak() const;

    u32 getId() const { return mId; }
    State getState() const { return mState; }
    bool isDone() const { return mState == State::cTerminated || mState == State::cReleased; }

    const CoreIdMask& getAffinity() const { return mAffinity; }
    void setAffinity(const CoreIdMask& affinity);

    static void yield();
    static void sleep(TickSpan howLong);

    void checkStackOverFlow(const char*, s32) const;
    void checkStackEndCorruption(const char*, s32) const;
    void checkStackPointerOverFlow(const char*, s32) const;
    void setStackOverflowExceptionEnable(bool);

    ThreadListNode* getThreadListNode() { return &mListNode; }

    static const s32 cDefaultPriority;

protected:
#ifdef NNSDK
    Thread(Heap* heap, nn::os::ThreadType*, u32);
#endif

    virtual void run_();
    virtual void calc_(MessageQueue::Element msg) = 0;
    virtual u64 getStackCheckStartAddress_() const;

    void initStackCheck_();
    void initStackCheckWithCurrentStackPointer_();

    static void ninThreadFunc_(void*);

    MessageQueue mMessageQueue;
    s32 mStackSize;
    ThreadListNode mListNode;
    Heap* mCurrentHeap;
    FindContainHeapCache mFindContainHeapCache;
    MessageQueue::BlockType mBlockType;
    MessageQueue::Element mQuitMsg;
    u32 mId;
    State mState;
    CoreIdMask mAffinity;
#ifdef NNSDK
    nn::os::ThreadType* mThreadInner;
#endif
    void* mStackTop2;
    void* mStackTop;
    s32 mPriority;
    s32 mCoreNo;  // XXX(leoetlino): this is in my BotW database - is this actually part of Thread?
};

class ThreadMgr : public hostio::Node
{
    SEAD_SINGLETON_DISPOSER(ThreadMgr)
public:
    ThreadMgr();
    virtual ~ThreadMgr();

    void initialize(Heap* heap);
    void destroy();

    bool isMainThread() const;
    Thread* getMainThread() const { return mMainThread; }
    Thread* getCurrentThread() const { return reinterpret_cast<Thread*>(mThreadPtrTLS.getValue()); }

    static void quitAndWaitDoneMultipleThread(Thread**, s32, bool);

    static void checkCurrentThreadStackOverFlow(const char*, s32);
    static void checkCurrentThreadStackEndCorruption(const char*, s32);
    static void checkCurrentThreadStackPointerOverFlow(const char*, s32);

    CriticalSection* getListCS() { return &mListCS; }

#ifdef SEAD_DEBUG
    void initHostIO();
    void genMessage(hostio::Context* context) override;
    void listenPropertyEvent(const hostio::PropertyEvent* event) override;
#endif

protected:
    friend class Thread;

    void addThread_(Thread* thread)
    {
        ScopedLock<CriticalSection> lock(getListCS());
        mList.pushBack(thread->getThreadListNode());
    }

    void removeThread_(Thread* thread)
    {
        ScopedLock<CriticalSection> lock(getListCS());
        mList.erase(thread->getThreadListNode());
    }

    void initMainThread_(Heap* heap);
    void destroyMainThread_();
    static u32 getCurrentThreadID_();

private:
    ThreadList mList;
    CriticalSection mListCS;
    Thread* mMainThread = nullptr;
    ThreadLocalStorage mThreadPtrTLS;
};
}  // namespace sead
