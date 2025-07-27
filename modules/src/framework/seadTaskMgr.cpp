#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskBase.h>
#include <framework/seadTaskMgr.h>
#include <prim/seadSafeString.h>
#include <resource/seadResourceMgr.h>
#include <thread/seadDelegateThread.h>

namespace sead
{
bool TaskMgr::changeTaskState_(TaskBase* task, TaskBase::State state)
{
    sead::ScopedLock<CriticalSection> lock{&mCriticalSection};

    if (task->mState == state)
        return false;

    switch (state)
    {
    case TaskBase::cPrepare:
        if (task->mState != TaskBase::cCreated)
            return false;

        task->mState = TaskBase::cPrepare;
        appendToList_(mPrepareList, task);

        if (mPrepareThread == nullptr ||
            mPrepareThread->sendMessage(1, MessageQueue::BlockType::NonBlocking))
        {
            return true;
        }

        return false;

    case TaskBase::cPrepareDone:
        task->mState = TaskBase::cPrepareDone;
        task->mTaskListNode.erase();

        return true;

    case TaskBase::cRunning:
        task->mState = TaskBase::cRunning;
        task->mTaskListNode.erase();
        appendToList_(mActiveList, task);

        task->enterCommon();

        return true;

    case TaskBase::cDying:
        task->mState = TaskBase::cDying;

        return true;

    case TaskBase::cDestroyable:
        if (task->mState != TaskBase::cRunning)
            return false;

        task->mState = TaskBase::cDestroyable;
        task->detachCalcImpl();
        task->detachDrawImpl();
        appendToList_(mDestroyableList, task);

        return true;

    case TaskBase::cDead:
        task->exit();
        task->mState = TaskBase::cDead;
        task->mTaskListNode.erase();

        return true;

    default:
        return false;
    }
}

void TaskMgr::destroyTaskSync(TaskBase* task)
{
    if (mParentFramework->mMethodTreeMgr->mCS.tryLock())
    {
        doDestroyTask_(task);
        mParentFramework->mMethodTreeMgr->mCS.unlock();
    }
}

void TaskMgr::doDestroyTask_(TaskBase* task)
{
    sead::ScopedLock<CriticalSection> lock{&mCriticalSection};

    TreeNode* node = task->child();
    while (node != nullptr)
    {
        doDestroyTask_(static_cast<TTreeNode<TaskBase*>*>(node)->value());
        node = task->child();
    }

    if (changeTaskState_(task, TaskBase::cDead))
    {
        task->detachAll();

        HeapArray heapArray(task->mHeapArray);
        s32 rootHeapNum = HeapMgr::getRootHeapNum();
        for (s32 i = 0; i < rootHeapNum; i++)
        {
            Heap* heap = heapArray.mHeaps[i];
            if (heap != nullptr)
                heap->destroy();
        }
    }
}

void TaskMgr::finalize()
{
    if (mPrepareThread != nullptr)
    {
        mPrepareThread->quitAndDestroySingleThread(false);
        delete mPrepareThread;
        mPrepareThread = nullptr;
    }

    if (mRootTask != nullptr)
    {
        destroyTaskSync(mRootTask);
        mRootTask = nullptr;
    }

    s32 rootHeapNum = HeapMgr::getRootHeapNum();
    for (s32 i = 0; i < rootHeapNum; i++)
    {
        Heap* heap = mHeapArray.mHeaps[i];
        if (heap)
        {
            heap->destroy();
            mHeapArray.mHeaps[i] = nullptr;
        }
    }
}

}  // namespace sead
