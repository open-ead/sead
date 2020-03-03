#ifndef SEAD_PTR_ARRAY_H_
#define SEAD_PTR_ARRAY_H_

namespace sead {

class PtrArrayImpl
{
public:
    __attribute__((always_inline)) PtrArrayImpl(s32 ptrNumMax, void* buf)
        : mSize(0)
        , mPtrNumMax(0)
        , mPtrs(NULL)
    {
        setBuffer(ptrNumMax, buf);
    }

    void setBuffer(s32 ptrNumMax, void* buf);

    s32 mSize;
    s32 mPtrNumMax;
    void* mPtrs;
};

} // namespace sead

#endif // SEAD_PTR_ARRAY_H_
