#pragma once

#include <basis/seadRawPrint.h>
#include <cmath>
#ifndef SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.h>
#endif
#include <prim/seadBitUtil.h>
#include <type_traits>

namespace sead
{
template <>
const MathCalcCommon<float>::SinCosSample MathCalcCommon<float>::cSinCosTbl[];

template <>
const MathCalcCommon<float>::AtanSample MathCalcCommon<float>::cAtanTbl[];

template <>
const MathCalcCommon<float>::ExpSample MathCalcCommon<float>::cExpTbl[];

template <>
const MathCalcCommon<float>::LogSample MathCalcCommon<float>::cLogTbl[];

template <typename T>
inline T MathCalcCommon<T>::sign(T value)
{
    return value < 0 ? -1 : 1;
}

template <typename T>
inline T MathCalcCommon<T>::sqrt(T t)
{
    return std::sqrt(t);
}

template <typename T>
inline T MathCalcCommon<T>::rsqrt(T t)
{
    return 1 / std::sqrt(t);
}

template <typename T>
inline T MathCalcCommon<T>::pow(T x, T y)
{
    return std::pow(x, y);
}

template <typename T>
inline T MathCalcCommon<T>::sin(T t)
{
    if constexpr (std::is_same_v<T, float>)
    {
        const auto as_int = BitUtil::bitCast<u32>(t);
        const SinCosSample& sample = cSinCosTbl[as_int >> 18];
        return sample.sin_val + sample.sin_delta * (as_int & 0xFFFFFFu) * 0x1p-24f;
    }
    else
    {
        static_assert(!std::is_same<T, T>(), "Unsupported type");
    }
}

template <typename T>
inline T MathCalcCommon<T>::cos(T t)
{
    if constexpr (std::is_same_v<T, float>)
    {
        const auto as_int = BitUtil::bitCast<u32>(t);
        const SinCosSample& sample = cSinCosTbl[as_int >> 18];
        return sample.cos_val + sample.cos_delta * (as_int & 0xFFFFFFu) * 0x1p-24f;
    }
    else
    {
        static_assert(!std::is_same<T, T>(), "Unsupported type");
    }
}

template <typename T>
inline T MathCalcCommon<T>::tan(T t)
{
    return std::tan(t);
}

template <typename T>
inline T MathCalcCommon<T>::asin(T t)
{
    if constexpr (std::is_floating_point<T>())
        SEAD_ASSERT_MSG(-1.0 <= t && t <= 1.0, "t(%f) is not in [-1, 1].", t);
    return std::asin(t);
}

template <typename T>
inline T MathCalcCommon<T>::acos(T t)
{
    if constexpr (std::is_floating_point<T>())
        SEAD_ASSERT_MSG(-1.0 <= t && t <= 1.0, "t(%f) is not in [-1, 1].", t);
    return std::acos(t);
}

template <typename T>
inline T MathCalcCommon<T>::atan(T t)
{
    return std::atan(t);
}

template <typename T>
inline T MathCalcCommon<T>::atan2(T y, T x)
{
    return std::atan2(y, x);
}

template <typename T>
inline T MathCalcCommon<T>::log2(T n)
{
    static_assert(std::is_integral<T>(), "T must be an integral type");
    return n <= 1 ? 0 : 1 + log2(n >> 1);
}

template <>
inline s32 MathCalcCommon<s32>::abs(s32 x)
{
    return (x ^ x >> 31) - (x >> 31);
}

template <>
inline u32 MathCalcCommon<u32>::abs(u32 x)
{
    return x;
}

template <>
inline f32 MathCalcCommon<f32>::abs(f32 x)
{
    return x > 0 ? x : -x;
}

template <typename T>
inline T MathCalcCommon<T>::max(T a, T b)
{
    return a < b ? b : a;
}

template <typename T>
inline T MathCalcCommon<T>::min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
inline T MathCalcCommon<T>::max3(T a, T b, T c)
{
    return max(max(a, b), c);
}

template <typename T>
inline T MathCalcCommon<T>::min3(T a, T b, T c)
{
    return min(min(a, b), c);
}

template <typename T>
inline T MathCalcCommon<T>::deg2rad(T deg)
{
    return deg * (numbers::pi_v<T> / static_cast<T>(180));
}

template <typename T>
inline T MathCalcCommon<T>::rad2deg(T rad)
{
    return rad * (static_cast<T>(180) / numbers::pi_v<T>);
}

template <typename T>
inline u32 MathCalcCommon<T>::deg2idx(T a)
{
    return static_cast<u32>(a * (cHalfRoundIdx / static_cast<T>(180)));
}

template <typename T>
inline u32 MathCalcCommon<T>::rad2idx(T a)
{
    return static_cast<u32>(a * (cHalfRoundIdx / pi()));
}

template <typename T>
inline T MathCalcCommon<T>::idx2deg(u32 a)
{
    return static_cast<T>(a * (static_cast<T>(180) / cHalfRoundIdx));
}

template <typename T>
inline T MathCalcCommon<T>::idx2rad(u32 a)
{
    return static_cast<T>(a * (pi() / cHalfRoundIdx));
}

template <typename T>
inline T MathCalcCommon<T>::roundUp(T x, s32 multNumber)
{
    SEAD_ASSERT(multNumber > 0);
    return (x + multNumber - 1) / multNumber * multNumber;
}

template <>
inline s32 MathCalcCommon<u32>::roundUpPow2(u32 val, s32 base)
{
    SEAD_ASSERT_MSG((u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

template <>
inline s32 MathCalcCommon<s32>::roundUpPow2(s32 val, s32 base)
{
    SEAD_ASSERT_MSG(val >= 0 && (u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]",
                    val, base);
    return val + base - 1 & (u32)-base;
}

template <typename T>
inline T MathCalcCommon<T>::clampMax(T val, T max_)
{
    return min(val, max_);
}

template <typename T>
inline T MathCalcCommon<T>::clampMin(T val, T min_)
{
    return max(val, min_);
}

template <typename T>
inline bool MathCalcCommon<T>::chase(T* value, T target, T step)
{
    const T current = *value;

    if (current < target)
    {
        const T new_value = current + step;
        if (target <= new_value || new_value < current)
        {
            *value = target;
            return true;
        }
        *value = new_value;
        return false;
    }

    if (current > target)
    {
        const T new_value = current - step;
        if (target >= new_value || current < new_value)
        {
            *value = target;
            return true;
        }
        *value = new_value;
        return false;
    }

    return true;
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
