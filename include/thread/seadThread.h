#pragma once

#ifdef NNSDK
#include <nn/os.h>
#endif

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeapMgr.h>
#include <mc/seadCoreInfo.h>
#include <prim/seadEnum.h>
#include <prim/seadNamable.h>
#include <prim/seadSafeString.hpp>
#include <thread/seadMessageQueue.h>
#include <time/seadTickSpan.h>

namespace sead
{
class Heap;
class Thread;

using ThreadListNode = TListNode<Thread*>;

class Thread : public IDisposer, public INamable
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
    const CoreIdMask& getAffinity() const { return mAffinity; }
    void setAffinity(const CoreIdMask& affinity);

    void yield();
    void sleep(TickSpan howLong);

    void checkStackOverFlow(const char*, s32) const;
    void checkStackEndCorruption(const char*, s32) const;
    void checkStackPointerOverFlow(const char*, s32) const;
    void setStackOverflowExceptionEnable(bool);

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
}  // namespace sead
