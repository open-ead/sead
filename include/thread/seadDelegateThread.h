#ifndef SEAD_DELEGATE_THREAD_H_
#define SEAD_DELEGATE_THREAD_H_

#include <thread/seadThread.h>

namespace sead {

template <typename T, typename U>
class IDelegate2;

class DelegateThread : public Thread
{
public:
    virtual ~DelegateThread();

    IDelegate2<Thread*, s32>* mDelegate; // IDelegate2<Thread*, MessageQueue::Element>*
};

} // namespace sead

#endif // SEAD_DELEGATE_THREAD_H_
