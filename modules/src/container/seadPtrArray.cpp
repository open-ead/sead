#include <cstring>

#include <basis/seadNew.h>
#include <container/seadPtrArray.h>
#include <sead/seadSystem.h>

namespace sead
{
void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax < 1)
    {
        SEAD_ASSERT(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
        return;
    }

    if (buf == NULL)
    {
        SEAD_ASSERT(false, "buf is null");
        return;
    }

    mPtrs = static_cast<void**>(buf);
    mPtrNum = 0;
    mPtrNumMax = ptrNumMax;
}

void PtrArrayImpl::allocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    SEAD_ASSERT_NOFMT(mPtrs == nullptr);

    if (ptrNumMax < 1)
    {
        SEAD_ASSERT(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
        return;
    }

    setBuffer(ptrNumMax, new (heap, alignment, std::nothrow) u8[s32(sizeof(void*)) * ptrNumMax]);
}

bool PtrArrayImpl::tryAllocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    SEAD_ASSERT_NOFMT(mPtrs == nullptr);

    if (ptrNumMax < 1)
    {
        SEAD_ASSERT(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
        return false;
    }

    auto* buf = new (heap, alignment, std::nothrow) u8[s32(sizeof(void*)) * ptrNumMax];
    if (!buf)
        return false;

    setBuffer(ptrNumMax, buf);
    return true;
}

void PtrArrayImpl::freeBuffer()
{
    if (mPtrs)
    {
        delete[] mPtrs;
        mPtrs = nullptr;
        mPtrNum = 0;
        mPtrNumMax = 0;
    }
}

void PtrArrayImpl::erase(s32 pos, s32 count)
{
    if (pos < 0)
    {
        SEAD_ASSERT(false, "illegal position[%d]", pos);
        return;
    }

    if (count < 0)
    {
        SEAD_ASSERT(false, "illegal number[%d]", count);
        return;
    }

    if (pos + count > mPtrNum)
    {
        SEAD_ASSERT(false, "pos[%d] + num[%d] exceed size[%d]", pos, count, mPtrNum);
        return;
    }

    const s32 endPos = pos + count;
    if (mPtrNum > endPos)
        std::memmove(mPtrs + pos, mPtrs + endPos, sizeof(void*) * (mPtrNum - endPos));

    mPtrNum -= count;
}

}  // namespace sead
