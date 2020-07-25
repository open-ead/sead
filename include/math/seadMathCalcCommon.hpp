#pragma once

#define SEAD_MATH_CALC_COMMON_HPP_

#include <basis/seadRawPrint.h>
#include <math/seadMathCalcCommon.h>

namespace sead
{
template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2(T val, s32 base)
{
    SEAD_ASSERT_MSG((u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

}  // namespace sead
