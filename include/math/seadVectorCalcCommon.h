#pragma once

#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
class Vector2CalcCommon
{
public:
    using Base = typename Policies<T>::Vec2Base;

public:
    static void add(Base& o, const Base& a, const Base& b);
    static void sub(Base& o, const Base& a, const Base& b);
    static void multScalar(Base& o, const Base& v, T t);

    static void negate(Base& v);
    static void set(Base& o, const Base& v);
    static void set(Base& v, T x, T y);

    static T dot(const Base& a, const Base& b);
    static T cross(const Base& a, const Base& b);
    static T squaredLength(const Base& v);
    static T length(const Base& v);
    static T normalize(Base& v);
};

template <typename T>
class Vector3CalcCommon
{
public:
    using Base = typename Policies<T>::Vec3Base;
    using Mtx33 = typename Policies<T>::Mtx33Base;
    using Mtx34 = typename Policies<T>::Mtx34Base;
    using Quat = typename Policies<T>::QuatBase;

    static void add(Base& o, const Base& a, const Base& b);
    static void sub(Base& o, const Base& a, const Base& b);
    /// Apply a rotation `m` to the vector `a`.
    static void mul(Base& o, const Mtx33& m, const Base& a);
    /// Apply a transformation `m` (rotation then translation) to the vector `a`.
    static void mul(Base& o, const Mtx34& m, const Base& a);

    /// Apply a rotation `m` to the vector `a`.
    static void rotate(Base& o, const Mtx33& m, const Base& a);
    /// Apply a rotation `m` to the vector `a`.
    static void rotate(Base& o, const Mtx34& m, const Base& a);
    /// Apply a rotation 'q' to the vector 'a'
    static void rotate(Base& o, const Quat& q, const Base& a);

    static void cross(Base& o, const Base& a, const Base& b);
    static T dot(const Base& a, const Base& b);
    static T squaredLength(const Base& v);
    static T length(const Base& v);
    static bool equals(const Base& lhs, const Base& rhs, T epsilon);
    static void multScalar(Base& o, const Base& v, T t);
    static void multScalarAdd(Base& o, T t, const Base& a, const Base& b);
    static T normalize(Base& v);
    static void negate(Base& v);
    static void set(Base& o, const Base& v);
    static void set(Base& v, T x, T y, T z);
};

template <typename T>
class Vector4CalcCommon
{
public:
    using Base = typename Policies<T>::Vec4Base;

public:
    static T normalize(Base& v);
    static void negate(Base& v);
    static T squaredLength(const Base& v);
    static T length(const Base& v);
    static void set(Base& o, const Base& v);
    static void set(Base& v, T x, T y, T z, T w);
};

}  // namespace sead

#define SEAD_MATH_VECTOR_CALC_COMMON_H_
#include <math/seadVectorCalcCommon.hpp>
#undef SEAD_MATH_VECTOR_CALC_COMMON_H_
