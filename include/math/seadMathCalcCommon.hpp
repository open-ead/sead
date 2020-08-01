#pragma once

#include <basis/seadRawPrint.h>
#ifndef SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.h>
#endif

namespace sead
{
template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2(T val, s32 base)
{
    SEAD_ASSERT_MSG((u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

}  // namespace sead
