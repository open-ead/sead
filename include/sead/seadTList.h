#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <sead/seadListImpl.h>

namespace sead
{
template <typename Pointer>
class TListNode;

template <typename Pointer>
class TList : public ListImpl
{
public:
    TList() : ListImpl() {}

    void insertFront(Pointer item)
    {
        item->mList = this;
        ListImpl::insertFront(item);
    }

    void erase(Pointer item)
    {
        item->mList = nullptr;
        ListImpl::erase(item);
    }

    TListNode<Pointer>* root() const { return static_cast<TListNode<Pointer>*>(mStartEnd.mNext); }

    static TListNode<Pointer>* next(TListNode<Pointer>* node)
    {
        return static_cast<TListNode<Pointer>*>(node->mNext);
    }

    bool isAtEnd(TListNode<Pointer>* node) const { return node == &mStartEnd; }
};

template <typename Pointer>
class TListNode : public ListNode
{
public:
    TListNode() : ListNode()
    {
        mData = static_cast<Pointer>(this);
        mList = NULL;
    }

    Pointer mData;
    TList<Pointer>* mList;
};

}  // namespace sead

#endif  // SEAD_TLIST_H_
