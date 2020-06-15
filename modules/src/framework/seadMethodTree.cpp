#include <framework/seadMethodTree.h>
#include <thread/seadCriticalSection.h>

namespace sead {

void
MethodTreeNode::attachMutexRec_(
    CriticalSection* m
) const
{
    const MethodTreeNode* node = this;

    do
    {
        MethodTreeNode* child = static_cast<MethodTreeNode*>(node->mChild);
        node->mCriticalSection = m;
        if (child != NULL && child->mData != NULL)
            child->mData->attachMutexRec_(m);
    }
    while (node->mNext != NULL && (node = static_cast<MethodTreeNode*>(node->mNext)->mData, node != NULL));
}

void MethodTreeNode::detachAll()
{
    CriticalSection* cs = mCriticalSection;
    attachMutexRec_(NULL);
    mCriticalSection = cs;

    lock_();
    TreeNode::detachAll();
    unlock_();

    mCriticalSection = NULL;
}

void MethodTreeNode::lock_()
{
    if (mCriticalSection == NULL)
        return;

    mCriticalSection->lock();
}

void MethodTreeNode::unlock_()
{
    if (mCriticalSection == NULL)
        return;

    mCriticalSection->unlock();
}

} // namespace sead
