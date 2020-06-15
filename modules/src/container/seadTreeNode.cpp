#include <container/seadTreeNode.h>

namespace sead {

void TreeNode::clearChildLinksRecursively_()
{
    TreeNode* node = this->mChild;
    while (node != NULL)
    {
        TreeNode* next = node->mNext;
        node->clearChildLinksRecursively_();
        node->clearLinks();
        node = next;
    }
}

void TreeNode::clearLinks()
{
    mPrev = NULL;
    mParent = NULL;
    mNext = NULL;
    mChild = NULL;
}

void TreeNode::detachAll()
{
    detachSubTree();
    clearChildLinksRecursively_();
    clearLinks();
}

void TreeNode::detachSubTree()
{
    if (mPrev != NULL)
    {
        mPrev->mNext = mNext;

        if (mNext != NULL)
        {
            mNext->mPrev = mPrev;
            mNext = NULL;
        }

        mPrev = NULL;
        mParent = NULL;

        return;
    }

    if (mParent != NULL)
    {
        mParent->mChild = mNext;
        mParent = NULL;
    }

    if (mNext == NULL)
        return;

    mNext->mPrev = mPrev;
    mNext = NULL;
}

} // namespace sead
