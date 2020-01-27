#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

namespace sead {

struct ListNode
{
    ListNode* previous;
    ListNode* next;
};

class ListImpl
{
public:
    ListNode root;
    s32 count;
    u32 nodeOffset;
};

class UnkList
{
public:
    UnkList()
    {
        ptrToSelf = this;
        root.next = nullptr;
        root.previous = nullptr;
        _C = 0;
    }

    ListNode root;
    void* ptrToSelf;
    u32 _C;
};

} // namespace sead

#endif // SEAD_LIST_IMPL_H_
