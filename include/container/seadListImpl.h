#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <basis/seadTypes.h>

namespace sead
{
class Random;

class ListNode
{
public:
    void insertFront_(ListNode* node);
    void erase_();

    bool isLinked() const { return mPrev || mNext; }

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

    void pushBack(ListNode* item)
    {
        mStartEnd.insertFront_(item);
        ++mCount;
    }

    ListNode* popBack();
    ListNode* popFront();

    void erase(ListNode* item)
    {
        item->erase_();
        --mCount;
    }

    void swap(ListNode* a, ListNode* b);
    void moveAfter(ListNode* a, ListNode* b);
    void moveBefore(ListNode* a, ListNode* b);

    void reverse();
    void shuffle(Random* random);
    void clear();

    int size() const { return mCount; }

    ListNode* nth(int n) const;
    s32 indexOf(const ListNode*) const;
    bool checkLinks() const;

protected:
    ListNode mStartEnd;
    s32 mCount;
};

}  // namespace sead

#endif  // SEAD_LIST_IMPL_H_
