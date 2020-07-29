#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <container/seadListImpl.h>

namespace sead
{
template <typename T>
class TListNode;

template <typename T>
class TList : public ListImpl
{
public:
    TList() : ListImpl() {}

    class iterator
    {
    public:
        iterator(TListNode<T>* ptr) : mPtr(ptr) {}

        iterator& operator++()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->next());
            return *this;
        }

        iterator& operator--()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->prev());
            return *this;
        }

        T& operator*() const { return mPtr->mData; }
        T* operator->() const { return &mPtr->mData; }

        friend bool operator==(iterator it1, iterator it2) { return it1.mPtr == it2.mPtr; }
        friend bool operator!=(iterator it1, iterator it2) { return !(it1 == it2); }

    private:
        TListNode<T>* mPtr;
    };

    iterator begin() const { return iterator(static_cast<TListNode<T>*>(mStartEnd.next())); }

    iterator end() const
    {
        return iterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd)));
    }

    void pushBack(TListNode<T>* item)
    {
        item->erase();
        item->mList = this;
        ListImpl::pushBack(item);
    }

    void erase(TListNode<T>* item)
    {
        item->mList = nullptr;
        ListImpl::erase(item);
    }

    TListNode<T>* root() const { return static_cast<TListNode<T>*>(mStartEnd.next()); }

    static TListNode<T>* next(TListNode<T>* node)
    {
        return static_cast<TListNode<T>*>(node->next());
    }

    bool isAtEnd(TListNode<T>* node) const { return node == &mStartEnd; }
};

template <typename T>
class TListNode : public ListNode
{
public:
    TListNode() : ListNode()
    {
        mData = static_cast<T>(this);
        mList = NULL;
    }

    TListNode(T data) : ListNode(), mData(data), mList(nullptr) {}

    void erase()
    {
        TList<T>* list = mList;
        if (list != NULL)
            list->erase(this);
    }

    T mData;
    TList<T>* mList;
};

}  // namespace sead

#endif  // SEAD_TLIST_H_
