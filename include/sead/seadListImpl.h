#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <sead/types.h>

namespace sead
{
class ListNode
{
public:
    void insertFront_(ListNode* node);
    void erase_();

    ListNode* mPrev = nullptr;
    ListNode* mNext = nullptr;
};

class ListImpl
{
public:
    __attribute__((always_inline)) ListImpl() : mStartEnd(), mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

    void insertFront(ListNode* item)
    {
        mStartEnd.insertFront_(item);
        ++mCount;
    }

    void erase(ListNode* item)
    {
        item->erase_();
        --mCount;
    }

    int size() const { return mCount; }

protected:
    ListNode mStartEnd;
    s32 mCount;
};

}  // namespace sead

#endif  // SEAD_LIST_IMPL_H_
