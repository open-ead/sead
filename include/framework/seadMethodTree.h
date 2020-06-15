#ifndef SEAD_METHODTREE_H_
#define SEAD_METHODTREE_H_

#include <container/seadTreeNode.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
//#include <prim/seadDelegate.h>
#include <prim/seadNamable.h>
#include <prim/seadRuntimeTypeInfo.h>
//#include <thread/seadCriticalSection.h>

namespace sead {

class CriticalSection;

template <typename T, typename U>
class IDelegate2;

class MethodTreeNode : public TTreeNode<MethodTreeNode*>, INamable, IDisposer
{
    SEAD_RTTI_BASE(MethodTreeNode)

public:
    enum PauseFlag
    {
        cPause_None = 0,
        cPause_Self = 1,
        cPause_Child = 2,
        cPause_Both = 3
    };

public:
    MethodTreeNode(CriticalSection* cs); /*
        : TTreeNode<MethodTreeNode*>()
        , INamable()
        , IDisposer()
        , mCriticalSection(cs)
        , mPauseFlag(0)
        , mPauseEventDelegate(NULL)
        , mUserID(NULL)
    {
        new (mDelegateHolder) Delegate<aaaaaaaaaaa>();
        mPauseFlag = BitFlag32(0);
    } */

    virtual ~MethodTreeNode()
    {
        detachAll();
    }

    void attachMutexRec_(CriticalSection* m) const;
    void detachAll();
    void lock_();
    void unlock_();

    typedef IDelegate2<MethodTreeNode*, PauseFlag> PauseEventDelegate;

    struct { u32 data_[4]; } mDelegateHolder;
    mutable CriticalSection* mCriticalSection;
    u32 mPriority;
    BitFlag32 mPauseFlag;
    PauseEventDelegate* mPauseEventDelegate;
    void* mUserID;
};

} // namespace sead

#endif // SEAD_METHODTREE_H_
