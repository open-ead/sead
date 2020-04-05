#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <sead/seadListImpl.h>

namespace sead {

class OffsetList : public ListImpl
{
public:
    inline ListNode* getNodeFromOffset(void* basePtr)
    {
        return static_cast<ListNode*>(basePtr + mOffset);
    }

    template <typename T>
    inline T getFromOffset(void* basePtr)
    {
        return static_cast<T>(basePtr + mOffset);
    }

    template <typename T>
    inline T getFromOffsetR(void* basePtr)
    {
        return static_cast<T>(basePtr - mOffset);
    }

    s32 mOffset;
};

} // namespace sead

#endif // SEAD_OFFSET_LIST_H_
