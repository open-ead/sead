#pragma once

#include <basis/seadTypes.h>

namespace sead
{
template <typename T>
class MathCalcCommon
{
public:
    static s32 roundUpPow2(T x, s32 y);

    static s32 roundDownPow2(T x, s32 y);
    static T gcd(T x, T y);
    static T lcm(T x, T y);
};

typedef MathCalcCommon<s32> MathCalcCommonS32;
typedef MathCalcCommon<u32> MathCalcCommonU32;

}  // namespace sead

#define SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.hpp>
#undef SEAD_MATH_MATH_CALC_COMMON_H_
