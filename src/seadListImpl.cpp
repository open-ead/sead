#include <sead/seadListImpl.h>
#include <sead/seadSystem.h>

namespace sead
{
void ListNode::insertFront_(ListNode* node)
{
    ListNode* prev = mPrev;
    this->mPrev = node;
    node->mPrev = prev;
    node->mNext = this;
    if (prev == NULL)
        return;

    prev->mNext = node;
}

void ListNode::erase_()
{
    SEAD_ASSERT(isLinked(), "node is not linked.");
    if (mPrev != nullptr)
        mPrev->mNext = mNext;

    if (mNext != nullptr)  // This disgusting cast is required for a 1:1 match...
        mNext->mPrev = static_cast<volatile ListNode*>(this)->mPrev;

    mPrev = mNext = NULL;
}

}  // namespace sead
