#include <prim/seadBitFlag.h>

namespace sead
{
int BitFlagUtil::countOnBit(u32 x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x += (x >> 8);
    x += (x >> 16);
    return x & 0x3f;
}

int BitFlagUtil::countRightOnBit(u32 x, int bit)
{
    SEAD_ASSERT(static_cast<u32>(bit) < sizeof(u32) * 8);
    const u32 mask = ((1u << bit) - 1) | (1u << bit);
    return countOnBit(x & mask);
}

int BitFlagUtil::findOnBitFromRight(u32 x, int num)
{
    SEAD_ASSERT(num > 0);
    if (!x)
        return -1;

    s32 i = num;
    const s32 result = -1;
    while (true)
    {
        --i;
        if (i <= 0)
            return countContinuousOffBitFromRight(x);
        x &= x - 1;
        if (!x)
            return result;
    }
}

int BitFlagUtil::countRightOnBit64(u64 x, int bit)
{
    SEAD_ASSERT(static_cast<u64>(bit) < sizeof(u64) * 8);
    const u64 mask = ((1ull << bit) - 1) | (1ull << bit);
    return countOnBit64(x & mask);
}

int BitFlagUtil::findOnBitFromRight64(u64 x, int num)
{
    SEAD_ASSERT(num > 0);
    if (!x)
        return -1;

    s32 i = num;
    const s32 result = -1;
    while (true)
    {
        --i;
        if (i <= 0)
            return countContinuousOffBitFromRight64(x);
        x &= x - 1;
        if (!x)
            return result;
    }
}


}  // namespace sead
