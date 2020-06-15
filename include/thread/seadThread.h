#ifndef SEAD_THREAD_H_
#define SEAD_THREAD_H_

#include <thread/seadThread.h>

namespace sead {

class Thread : public IDisposer, public INamable
{
public:
    virtual ~Thread();

    virtual void destroy();
    virtual bool sendMessage(s32, u32); // sendMessage(MessageQueue::Element, MessageQueue::BlockType)
    virtual s32 recvMessage(u32); // MessageQueue::Element recvMessage(MessageQueue::BlockType)
    virtual bool start();
    virtual void quit(bool);
    virtual void waitDone();
    virtual void quitAndDestroySingleThread(bool is_jam);
    //...
};

} // namespace sead

#endif // SEAD_THREAD_H_
