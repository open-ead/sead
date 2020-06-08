#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <sead/seadListImpl.h>
#include <sead/types.h>

namespace sead
{
template <typename T>
class OffsetList : public ListImpl
{
public:
    OffsetList(s32 offset) : mOffset{offset} {}

    class iterator
    {
    public:
        iterator(T* ptr, s32 offset) : mPtr{ptr}, mOffset{offset} {}
        bool operator==(const iterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        iterator& operator++()
        {
            advance();
            return *this;
        }
        iterator operator++(int)
        {
            iterator old = *this;
            advance();
            return old;
        }
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

    private:
        void advance() { mPtr = ObjFromNode(NodeFromObj(mPtr, mOffset)->mNext, mOffset); }

        T* mPtr;
        s32 mOffset;
    };

    iterator begin() { return iterator(ObjFromNode(mStartEnd.mNext, mOffset), mOffset); }
    iterator end() { return iterator(ObjFromNode(&mStartEnd, mOffset), mOffset); }

    void insertFront(T* item) { ListImpl::insertFront(NodeFromObj(item, mOffset)); }

    void erase(T* item) { ListImpl::erase(NodeFromObj(item, mOffset)); }

private:
    static ListNode* NodeFromObj(T* ptr, int offset)
    {
        return reinterpret_cast<ListNode*>(reinterpret_cast<u8*>(ptr) + offset);
    }

    static T* ObjFromNode(ListNode* node, int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<u8*>(node) - offset);
    }

    s32 mOffset;
};

}  // namespace sead

#endif  // SEAD_OFFSET_LIST_H_
