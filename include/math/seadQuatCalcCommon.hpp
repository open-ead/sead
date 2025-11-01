#pragma once

#ifndef SEAD_MATH_QUAT_CALC_COMMON_H_
#include <math/seadQuatCalcCommon.h>
#endif

#include <limits>
#include <math/seadMathCalcCommon.h>
#include <math/seadVectorCalcCommon.h>

namespace sead
{
template <typename T>
inline T QuatCalcCommon<T>::length(const Base& q)
{
    return MathCalcCommon<T>::sqrt(squaredLength(q));
}

template <typename T>
inline T QuatCalcCommon<T>::squaredLength(const Base& q)
{
    return (q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z);
}

template <typename T>
inline T QuatCalcCommon<T>::normalize(Base& q)
{
    const T len = length(q);
    if (len > 0)
    {
        const T inv_len = T(1) / len;
        q.w *= inv_len;
        q.x *= inv_len;
        q.y *= inv_len;
        q.z *= inv_len;
    }

    return len;
}

template <typename T>
inline T QuatCalcCommon<T>::dot(const Base& u, const Base& v)
{
    return (u.w * v.w) + (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

template <typename T>
inline void QuatCalcCommon<T>::add(Base& out, const Base& u, const Base& v)
{
    out.w = u.w + v.w;
    out.x = u.x + v.x;
    out.y = u.y + v.y;
    out.z = u.z + v.z;
}

template <typename T>
inline void QuatCalcCommon<T>::sub(Base& out, const Base& u, const Base& v)
{
    out.w = u.w - v.w;
    out.x = u.x - v.x;
    out.y = u.y - v.y;
    out.z = u.z - v.z;
}

template <typename T>
inline void QuatCalcCommon<T>::setMul(Base& out, const Base& u, const Base& v)
{
    T w = (u.w * v.w) - (u.x * v.x) - (u.y * v.y) - (u.z * v.z);
    T x = (u.w * v.x) + (u.x * v.w) + (u.y * v.z) - (u.z * v.y);
    T y = (u.w * v.y) - (u.x * v.z) + (u.y * v.w) + (u.z * v.x);
    T z = (u.w * v.z) + (u.x * v.y) - (u.y * v.x) + (u.z * v.w);
    out.w = w;
    out.x = x;
    out.y = y;
    out.z = z;
}

template <typename T>
inline void QuatCalcCommon<T>::setMulScalar(Base& out, const Base& q, T t)
{
    out.w = q.w * t;
    out.x = q.x * t;
    out.y = q.y * t;
    out.z = q.z * t;
}

template <typename T>
inline void QuatCalcCommon<T>::setInverse(Base& out, const Base& q)
{
    T prod = squaredLength(q);
    if (prod > std::numeric_limits<T>::epsilon())
    {
        T inv = 1 / prod;
        out.w = inv * q.w;
        out.x = inv * -q.x;
        out.y = inv * -q.y;
        out.z = inv * -q.z;
    }
    else
    {
        out.w = q.w;
        out.x = -q.x;
        out.y = -q.y;
        out.z = -q.z;
    }
}

template <typename T>
inline void QuatCalcCommon<T>::slerpTo(Base& out, const Base& q1, const Base& q2, f32 t)
{
    T dot = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
    if (dot > 1)
        dot = 1;
    else if (dot < -1)
        dot = -1;

    const T dot_0 = dot;
    if (dot < 0)
        dot = -dot;

    T a, b;
    const T theta_0 = MathCalcCommon<T>::acos(dot);
    const T sin_theta_0 = std::sin(theta_0);
    if (MathCalcCommon<T>::abs(sin_theta_0) < std::numeric_limits<T>::epsilon())
    {
        a = 1.0f - t;
        b = t;
    }
    else
    {
        const T theta = theta_0 * t;
        const T sin_theta_0_inv = 1.0f / sin_theta_0;
        a = sin_theta_0_inv * std::sin(theta_0 - theta);
        b = sin_theta_0_inv * std::sin(theta);
    }

    if (dot_0 < 0)
        b = -b;

    out.x = a * q1.x + b * q2.x;
    out.y = a * q1.y + b * q2.y;
    out.z = a * q1.z + b * q2.z;
    out.w = a * q1.w + b * q2.w;
}

template <typename T>
inline void QuatCalcCommon<T>::makeUnit(Base& q)
{
    q = {0, 0, 0, 1};
}

template <typename T>
inline bool QuatCalcCommon<T>::makeVectorRotation(Base& q, const Vec3& from, const Vec3& to)
{
    // Based on The Shortest Arc Quaternion from Game Programming Gems 1
    Vec3 cross;
    Vector3CalcCommon<T>::cross(cross, from, to);
    const T dot = Vector3CalcCommon<T>::dot(from, to) + 1;

    if (dot <= MathCalcCommon<T>::epsilon())
    {
        makeUnit(q);
        return false;
    }
    else
    {
        T s = MathCalcCommon<T>::sqrt(2 * dot);
        T rs = 1 / s;
        set(q, s * 0.5f, cross.x * rs, cross.y * rs, cross.z * rs);
        return true;
    }
}

template <typename T>
inline void QuatCalcCommon<T>::set(Base& q, const Base& other)
{
    q = other;
}

template <typename T>
inline void QuatCalcCommon<T>::set(Base& q, T w, T x, T y, T z)
{
    q.w = w;
    q.x = x;
    q.y = y;
    q.z = z;
}

template <typename T>
inline void QuatCalcCommon<T>::setRPY(Base& q, T roll, T pitch, T yaw)
{
    const T cy = std::cos(yaw / 2);
    const T cp = std::cos(pitch / 2);
    const T cr = std::cos(roll / 2);
    const T sy = std::sin(yaw / 2);
    const T sp = std::sin(pitch / 2);
    const T sr = std::sin(roll / 2);

    const T cy_cp = cy * cp;
    const T sy_sp = sy * sp;
    const T cy_sp = cy * sp;
    const T sy_cp = sy * cp;

    q.w = (cy_cp * cr) + (sy_sp * sr);
    q.x = (cy_cp * sr) - (sy_sp * cr);
    q.y = (cy_sp * cr) + (sy_cp * sr);
    q.z = (sy_cp * cr) - (cy_sp * sr);
}

template <typename T>
inline void QuatCalcCommon<T>::setAxisAngle(Base& q, const Vec3& axis, T angle)
{
    setAxisRadian(q, axis, MathCalcCommon<T>::deg2rad(angle));
}

template <typename T>
inline void QuatCalcCommon<T>::setAxisRadian(Base& q, const Vec3& axis, T radian)
{
    radian *= 0.5f;
    q.w = MathCalcCommon<T>::cos(radian);
    T sa = MathCalcCommon<T>::sin(radian);
    q.x = sa * axis.x;
    q.y = sa * axis.y;
    q.z = sa * axis.z;
}

}  // namespace sead
