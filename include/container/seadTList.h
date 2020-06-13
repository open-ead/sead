#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <container/seadListImpl.h>

namespace sead
{
template <typename Pointer>
class TListNode;

template <typename Pointer>
class TList : public ListImpl
{
public:
    TList() : ListImpl() {}

    class iterator
    {
    public:
        iterator(TListNode<Pointer>* ptr) : mPtr(ptr) {}

        iterator& operator++()
        {
            mPtr = static_cast<TListNode<Pointer>*>(mPtr->next());
            return *this;
        }

        iterator operator++(int)
        {
            const iterator it(*this);
            (void)++*this;
            return it;
        }

        iterator& operator--()
        {
            mPtr = static_cast<TListNode<Pointer>*>(mPtr->prev());
            return *this;
        }

        iterator operator--(int)
        {
            const iterator it(*this);
            (void)--*this;
            return it;
        }

        friend bool operator==(iterator it1, iterator it2) { return it1.mPtr == it2.mPtr; }

        friend bool operator!=(iterator it1, iterator it2) { return !(it1 == it2); }

        TListNode<Pointer>* mPtr;
    };

    iterator begin() const { return iterator(static_cast<TListNode<Pointer>*>(mStartEnd.next())); }

    iterator end() const
    {
        return iterator(static_cast<TListNode<Pointer>*>(const_cast<ListNode*>(&mStartEnd)));
    }

    void insertFront(Pointer item)
    {
        item->mList = this;
        ListImpl::pushBack(item);
    }

    void erase(Pointer item)
    {
        item->mList = nullptr;
        ListImpl::erase(item);
    }

    TListNode<Pointer>* root() const { return static_cast<TListNode<Pointer>*>(mStartEnd.next()); }

    static TListNode<Pointer>* next(TListNode<Pointer>* node)
    {
        return static_cast<TListNode<Pointer>*>(node->next());
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
