#ifndef SEAD_PTR_ARRAY_H_
#define SEAD_PTR_ARRAY_H_

#include <sead/types.h>

namespace sead {

class PtrArrayImpl
{
public:
    __attribute__((always_inline)) PtrArrayImpl(s32 ptrNumMax, void* buf)
        : mPtrNum(0)
        , mPtrNumMax(0)
        , mPtrs(NULL)
    {
        setBuffer(ptrNumMax, buf);
    }

    void setBuffer(s32 ptrNumMax, void* buf);

    s32 mPtrNum;
    s32 mPtrNumMax;
    void** mPtrs;
};

} // namespace sead

#endif // SEAD_PTR_ARRAY_H_
