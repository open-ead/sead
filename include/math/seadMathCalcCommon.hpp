#pragma once

#include <basis/seadRawPrint.h>
#ifndef SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.h>
#endif

namespace sead
{
template <typename T>
inline T MathCalcCommon<T>::roundUp(T x, s32 multNumber)
{
    SEAD_ASSERT(multNumber > 0);
    return (x + multNumber - 1) / multNumber * multNumber;
}

template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2(T val, s32 base)
{
    SEAD_ASSERT_MSG((u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2Positive(T val, s32 base)
{
    SEAD_ASSERT_MSG(val >= 0 && (u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]",
                    val, base);
    return val + base - 1 & (u32)-base;
}

template <typename T, typename T2>
inline T lerp(T a, T b, T2 t)
{
    return a + (b - a) * t;
}

template <typename T>
inline T clamp(T value, T low, T high)
{
    if (value < low)
        value = low;
    else if (value > high)
        value = high;
    return value;
}
}  // namespace sead
