#include <algorithm>
#include <basis/seadNew.h>
#include <basis/seadRawPrint.h>
#include <container/seadPtrArray.h>

namespace sead
{
void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax < 1)
    {
        SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
        return;
    }

    if (buf == NULL)
    {
        SEAD_ASSERT_MSG(false, "buf is null");
        return;
    }

    mPtrs = static_cast<void**>(buf);
    mPtrNum = 0;
    mPtrNumMax = ptrNumMax;
}

void PtrArrayImpl::allocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    SEAD_ASSERT(mPtrs == nullptr);

    if (ptrNumMax < 1)
    {
        SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
        return;
    }

    setBuffer(ptrNumMax, new (heap, alignment, std::nothrow) u8[s32(sizeof(void*)) * ptrNumMax]);
}

bool PtrArrayImpl::tryAllocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    SEAD_ASSERT(mPtrs == nullptr);

    if (ptrNumMax < 1)
    {
        SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
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
    if (isBufferReady())
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
        SEAD_ASSERT_MSG(false, "illegal position[%d]", pos);
        return;
    }

    if (count < 0)
    {
        SEAD_ASSERT_MSG(false, "illegal number[%d]", count);
        return;
    }

    if (pos + count > mPtrNum)
    {
        SEAD_ASSERT_MSG(false, "pos[%d] + num[%d] exceed size[%d]", pos, count, mPtrNum);
        return;
    }

    const s32 endPos = pos + count;
    if (mPtrNum > endPos)
        MemUtil::copyOverlap(mPtrs + pos, mPtrs + endPos, sizeof(void*) * (mPtrNum - endPos));

    mPtrNum -= count;
}

void PtrArrayImpl::reverse()
{
    const s32 half = mPtrNum / 2;
    for (s32 i = 0; i < half; ++i)
        swap(mPtrNum - i - 1, i);
}

// Fisher–Yates shuffle.
void PtrArrayImpl::shuffle(Random* random)
{
    SEAD_ASSERT(random);
    for (s32 i = mPtrNum - 1; i > 0; --i)
        swap(i, random->getS32Range(0, i + 1));
}

void PtrArrayImpl::insert(s32 pos, void* ptr)
{
    if (!checkInsert(pos, 1))
        return;

    createVacancy(pos, 1);
    mPtrs[pos] = ptr;
    ++mPtrNum;
}

bool PtrArrayImpl::checkInsert(s32 pos, s32 num)
{
    if (pos < 0)
    {
        SEAD_ASSERT_MSG(false, "illegal position[%d]", pos);
        return false;
    }

    if (mPtrNum + num > mPtrNumMax)
    {
        SEAD_ASSERT_MSG(false, "list is full.");
        return false;
    }

    if (mPtrNum < pos)
    {
        SEAD_ASSERT_MSG(false, "pos[%d] exceed size[%d]", pos, mPtrNum);
        return false;
    }

    return true;
}

// TODO: PtrArrayImpl::insertArray

void PtrArrayImpl::sort(CompareCallbackImpl cmp)
{
    if (mPtrNum < 2)
        return;

    void** ptrs = mPtrs;
    s32 begin = 0;
    s32 end = mPtrNum - 1;
    do
    {
        s32 last = begin;
        for (s32 i = begin; i < end; ++i)
        {
            if (cmp(ptrs[i], ptrs[i + 1]) > 0)
            {
                void* tmp = ptrs[i + 1];
                ptrs[i + 1] = ptrs[i];
                ptrs[i] = tmp;
                last = i;
            }
        }

        end = last;
        if (begin == end)
            break;

        last = end;
        for (s32 i = end; i > begin; --i)
        {
            if (cmp(ptrs[i], ptrs[i - 1]) < 0)
            {
                void* tmp = ptrs[i - 1];
                ptrs[i - 1] = ptrs[i];
                ptrs[i] = tmp;
                last = i;
            }
        }
        begin = last;
    } while (begin != end);
}

// TODO: PtrArrayImpl::heapSort

// TODO: PtrArrayImpl::compare

// TODO: PtrArrayImpl::uniq

// TODO: PtrArrayImpl::binarySearch

}  // namespace sead
