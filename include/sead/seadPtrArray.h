#ifndef SEAD_PTR_ARRAY_H_
#define SEAD_PTR_ARRAY_H_

#include <sead/seadSystem.h>
#include <sead/types.h>

namespace sead
{
class PtrArrayImpl
{
public:
    __attribute__((always_inline)) PtrArrayImpl(s32 ptrNumMax, void* buf)
        : mPtrNum(0), mPtrNumMax(0), mPtrs(NULL)
    {
        setBuffer(ptrNumMax, buf);
    }

    void setBuffer(s32 ptrNumMax, void* buf);

    s32 mPtrNum;
    s32 mPtrNumMax;
    void** mPtrs;
};

template <typename T>
class PtrArray : public PtrArrayImpl
{
public:
    T*& operator[](size_t idx)
    {
        SEAD_ASSERT(idx < size_t(mPtrNum), "index exceeded [%d/%d]", int(idx), mPtrNum);
        return static_cast<T*>(idx < size_t(mPtrNum) ? mPtrs[idx] : mPtrs[0]);
    }

    T* operator[](size_t idx) const
    {
        SEAD_ASSERT(idx < size_t(mPtrNum), "index exceeded [%d/%d]", int(idx), mPtrNum);
        return static_cast<T*>(idx < size_t(mPtrNum) ? mPtrs[idx] : mPtrs[0]);
    }
};

// TODO: sead::PtrArray::iterator
// TODO: sead::PtrArray::constIterator
// TODO: sead::FixedPtrArray : sead::PtrArray

}  // namespace sead

#endif  // SEAD_PTR_ARRAY_H_
