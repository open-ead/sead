#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>
#include <container/seadListImpl.h>
#include <prim/seadPtrUtil.h>

namespace sead
{
template <typename T>
class OffsetList : public ListImpl
{
public:
    OffsetList() = default;

    void initOffset(s32 offset) { mOffset = offset; }

    void clear();

    void pushBack(T* item)
    {
        SEAD_ASSERT_NOFMT(mOffset >= 0);
        ListImpl::pushBack(objToListNode(item));
    }

    void pushFront(T* item)
    {
        SEAD_ASSERT_NOFMT(mOffset >= 0);
        ListImpl::pushFront(objToListNode(item));
    }

    T* popBack();
    T* popFront();

    void insertBefore(const T*, T*);
    void insertAfter(const T*, T*);
    void erase(T* item) { ListImpl::erase(objToListNode(item)); }

    T* front() const;
    T* back() const;
    T* prev(const T*) const;
    T* next(const T*) const;
    T* nth(int) const;
    int indexOf(const T*) const;

    bool isNodeLinked(const T*) const;

    void swap(T*, T*);
    void moveAfter(T*, T*);
    void moveBefore(T*, T*);

    using CompareCallback = int (*)(const T*, const T*);

    void sort();
    void sort(CompareCallback);
    void mergeSort();
    void mergeSort(CompareCallback);

    T* find(const T*) const;
    T* find(const T*, CompareCallback) const;

    void uniq();
    void uniq(CompareCallback);

    class iterator
    {
    public:
        iterator(T* ptr, s32 offset) : mPtr{ptr}, mOffset{offset} {}
        bool operator==(const iterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        iterator& operator++()
        {
            ListNode* node = static_cast<ListNode*>(PtrUtil::addOffset(mPtr, mOffset))->next();
            mPtr = static_cast<T*>(PtrUtil::addOffset(node, -mOffset));
            return *this;
        }
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

    private:
        T* mPtr;
        s32 mOffset;
    };

    iterator begin() const { return iterator(listNodeToObj(mStartEnd.next()), mOffset); }
    iterator end() const
    {
        return iterator(listNodeToObj(const_cast<ListNode*>(&mStartEnd)), mOffset);
    }

protected:
    static int compareT(const T*, const T*);

    ListNode* objToListNode(T* obj) const
    {
        return static_cast<ListNode*>(PtrUtil::addOffset(obj, mOffset));
    }

    T* listNodeToObj(ListNode* node) const
    {
        return static_cast<T*>(PtrUtil::addOffset(node, -mOffset));
    }

    s32 mOffset = -1;
};

}  // namespace sead

#endif  // SEAD_OFFSET_LIST_H_
