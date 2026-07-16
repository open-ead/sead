#ifndef SEAD_METHODTREE_H_
#define SEAD_METHODTREE_H_

#include <container/seadTreeNode.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
#include <prim/seadDelegate.h>
#include <prim/seadNamable.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadStorageFor.h>

namespace sead
{
class CriticalSection;

template <typename T, typename U>
class IDelegate2;

class MethodTreeNode : public TTreeNode<MethodTreeNode*>, public INamable, public IDisposer
{
    SEAD_RTTI_BASE(MethodTreeNode)

public:
    enum PauseFlag
    {
        cPause_None = 0,
        cPause_Self = 1,
        cPause_Child = 2,
        cPause_Both = 3,
    };

    using PauseEventDelegate = IDelegate2<MethodTreeNode*, PauseFlag>;

    explicit MethodTreeNode(CriticalSection* cs) : TTreeNode(this)
    {
        mPauseEventDelegate = nullptr;
        mCriticalSection = cs;
        mDelegateHolder.construct(sead::Delegate<MethodTreeNode>());
        mUserID = nullptr;
        mPauseFlag = cPause_None;
        setPauseFlag(cPause_Both);
    }

    ~MethodTreeNode() override { detachAll(); }

    template <typename Delegate>
    void bind(const Delegate& delegate, const char* name)
    {
        lock_();
        mDelegateHolder.construct(delegate);
        unlock_();

        if (name)
            setName(name);
    }

    void call();
    void detachAll();
    void pushBackChild(MethodTreeNode* node);
    void pushFrontChild(MethodTreeNode* node);

    void setPauseFlag(PauseFlag flag)
    {
        lock_();
        if (mPauseEventDelegate)
            mPauseEventDelegate->invoke(this, flag);
        mPauseFlag = flag;
        unlock_();
    }

private:
    void attachMutexRec_(CriticalSection* m) const;
    void callRec_();

    void lock_();
    void unlock_();

    StorageFor<sead::AnyDelegate> mDelegateHolder;
    mutable CriticalSection* mCriticalSection;
    [[maybe_unused]] u32 mPriority;
    BitFlag32 mPauseFlag;
    PauseEventDelegate* mPauseEventDelegate;
    void* mUserID;
};

}  // namespace sead

#endif  // SEAD_METHODTREE_H_
