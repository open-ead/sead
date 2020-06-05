#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <sead/seadListImpl.h>
#include <sead/types.h>

namespace sead {

class OffsetList : public ListImpl
{
public:
    inline ListNode* getNodeFromOffset(void* basePtr)
    {
        return reinterpret_cast<ListNode*>(static_cast<u8*>(basePtr) + mOffset);
    }

    template <typename T>
    inline T getFromOffset(void* basePtr)
    {
        return reinterpret_cast<T>(static_cast<u8*>(basePtr) + mOffset);
    }

    template <typename T>
    inline T getFromOffsetR(void* basePtr)
    {
        return reinterpret_cast<T>(static_cast<u8*>(basePtr) - mOffset);
    }

    s32 mOffset;
};

} // namespace sead

#endif // SEAD_OFFSET_LIST_H_
