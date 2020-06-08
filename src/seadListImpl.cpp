#include <sead/seadListImpl.h>

namespace sead
{
void ListNode::insertFront_(ListNode* node)
{
    ListNode* prev = mPrev;
    this->mPrev = node;
    node->mNext = this;
    node->mPrev = prev;
    if (prev == NULL)
        return;

    prev->mNext = node;
}

void ListNode::erase_()
{
    if (this->mPrev != NULL)
        this->mPrev->mNext = this->mNext;

    if (this->mNext != NULL)
        this->mNext->mPrev = this->mPrev;

    this->mNext = NULL;
    this->mPrev = NULL;
}

}  // namespace sead
