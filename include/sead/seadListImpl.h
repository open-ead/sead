#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <sead/types.h>

namespace sead
{
class ListNode
{
public:
    __attribute__((always_inline)) ListNode() : mPrev(NULL), mNext(NULL) {}

    void insertFront_(ListNode* node);
    void erase_();

    ListNode* mPrev;
    ListNode* mNext;
};

class ListImpl
{
public:
    __attribute__((always_inline)) ListImpl() : mStartEnd(), mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

    ListNode mStartEnd;
    s32 mCount;
};

}  // namespace sead

#endif  // SEAD_LIST_IMPL_H_
