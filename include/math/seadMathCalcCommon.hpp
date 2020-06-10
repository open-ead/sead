#pragma once

#include <math/seadMathCalcCommon.h>

namespace sead {

template <typename T>
inline s32
MathCalcCommon<T>::roundUpPow2(T x, s32 y)
{
    return x + y - 1 & (u32)-y;
}

}
