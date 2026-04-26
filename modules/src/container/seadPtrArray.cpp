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
    s32 size = mPtrNum / 2;
    for (s32 i = 0; i < size; ++i)
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

// NON-MATCHING: Inverted registers https://decomp.me/scratch/fBexb
void PtrArrayImpl::insertArray(s32 idx, void* array, s32 arrayLength, s32 elementSize)
{
    if (!checkInsert(idx, arrayLength))
        return;

    createVacancy(idx, arrayLength);

    for (s32 i = 0; i < arrayLength; ++i)
        mPtrs[idx + i] = (u8*)array + (elementSize * i);

    mPtrNum += arrayLength;
}

// Bidirectional bubble sort
void PtrArrayImpl::sort(CompareCallbackImpl cmp)
{
    if (mPtrNum < 2)
        return;

    void** ptrs = mPtrs;
    s32 left = 0;
    s32 right = mPtrNum - 1;

    do
    {
        s32 lastSwapForward = left;
        for (s32 i = left; i < right; ++i)
        {
            if (cmp(ptrs[i], ptrs[i + 1]) > 0)
            {
                void* temp = ptrs[i + 1];
                ptrs[i + 1] = ptrs[i];
                ptrs[i] = temp;
                lastSwapForward = i;
            }
        }
        right = lastSwapForward;

        if (left == right)
            break;

        s32 lastSwapBackward = right;
        for (s32 i = right; i > left; --i)
        {
            if (cmp(ptrs[i], ptrs[i - 1]) < 0)
            {
                void* temp = ptrs[i - 1];
                ptrs[i - 1] = ptrs[i];
                ptrs[i] = temp;
                lastSwapBackward = i;
            }
        }
        left = lastSwapBackward;
    } while (left != right);
}

void PtrArrayImpl::heapSort(CompareCallbackImpl cmp)
{
    if (mPtrNum < 2)
        return;

    void** ptrs = mPtrs;
    s32 i = mPtrNum / 2;
    s32 n = mPtrNum;

    do
    {
        s32 parent = i--;
        void* val = ptrs[parent - 1];

        while (parent * 2 <= n)
        {
            s32 child = parent * 2;
            if (child < n && cmp(ptrs[child - 1], ptrs[child]) < 0)
                child++;

            if (cmp(val, ptrs[child - 1]) >= 0)
                break;

            ptrs[parent - 1] = ptrs[child - 1];
            parent = child;
        }

        ptrs[parent - 1] = val;
    } while (i > 0);

    while (n > 1)
    {
        void* val = ptrs[n - 1];
        ptrs[--n] = ptrs[0];

        s32 parent = 1;
        while (parent * 2 <= n)
        {
            s32 child = parent * 2;
            if (child < n && cmp(ptrs[child - 1], ptrs[child]) < 0)
                child++;

            if (cmp(val, ptrs[child - 1]) >= 0)
                break;

            ptrs[parent - 1] = ptrs[child - 1];
            parent = child;
        }
        ptrs[parent - 1] = val;
    }
}

s32 PtrArrayImpl::compare(const PtrArrayImpl& other, CompareCallbackImpl cmp) const
{
    for (s32 i = 0; i < size(); i++)
    {
        if (i >= other.size())
            return 1;

        const s32 c = cmp(mPtrs[i], other.unsafeAt(i));
        if (c != 0)
            return c;
    }

    return size() != other.size() ? -1 : 0;
}

void PtrArrayImpl::uniq(CompareCallbackImpl cmp)
{
    for (s32 i = 0; i < size() - 1; i++)
    {
        for (s32 e = i + 1; e < size();)
        {
            if (cmp(mPtrs[i], mPtrs[e]) == 0)
                erase(e, 1);
            else
                e++;
        }
    }
}

s32 PtrArrayImpl::binarySearch(const void* ptr, CompareCallbackImpl cmp) const
{
    if (mPtrNum == 0)
        return -1;

    s32 low = 0;
    s32 high = size() - 1;
    while (low < high)
    {
        const s32 mid = (low + high) / 2;
        const s32 c = cmp(mPtrs[mid], ptr);
        if (c == 0)
            return mid;
        if (c < 0)
            low = mid + 1;
        else
            high = mid;
    }

    if (cmp(mPtrs[low], ptr) == 0)
        return low;

    return -1;
}

}  // namespace sead
