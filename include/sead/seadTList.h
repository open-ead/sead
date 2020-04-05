#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <sead/seadListImpl.h>

namespace sead {

template <typename T>
class TListNode;

template <typename T>
class TList : public ListImpl
{
public:
    TList()
        : ListImpl()
    {
    }

    // Self-defined inlines for convenience

    void setAsListFor(T* item)
    {
        item->mList = this;
    }

    void insertFront(T* item)
    {
        mStartEnd.insertFront_(item);
    }

    TListNode<T>* root() const
    {
        return static_cast<TListNode<T>*>(mStartEnd.mNext);
    }

    static TListNode<T>* next(TListNode<T>* node)
    {
        return static_cast<TListNode<T>*>(node->mNext);
    }

    bool isAtEnd(TListNode<T>* node) const
    {
        return node == &mStartEnd;
    }
};

template <typename T>
class TListNode : public ListNode
{
public:
    TListNode()
        : ListNode()
    {
        mData = static_cast<T*>(this);
        mList = NULL;
    }

    T* mData;
    TList<T>* mList;
};

} // namespace sead

#endif // SEAD_TLIST_H_
