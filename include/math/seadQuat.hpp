#pragma once

#include <math/seadQuatCalcCommon.h>
#ifndef SEAD_MATH_QUAT_H_
#include <math/seadQuat.h>
#endif

namespace sead
{
template <typename T>
inline Quat<T>::Quat(T w_, T x_, T y_, T z_)
{
    QuatCalcCommon<T>::set(*this, w_, x_, y_, z_);
}

template <typename T>
inline Quat<T>& Quat<T>::operator+=(const Quat& other)
{
    QuatCalcCommon<T>::add(*this, *this, other);
    return *this;
}

template <typename T>
inline Quat<T>& Quat<T>::operator-=(const Quat& other)
{
    QuatCalcCommon<T>::sub(*this, *this, other);
    return *this;
}

template <typename T>
inline Quat<T>& Quat<T>::operator*=(const Quat& other)
{
    QuatCalcCommon<T>::setMul(*this, *this, other);
    return *this;
}

template <typename T>
inline Quat<T>& Quat<T>::operator*=(T t)
{
    QuatCalcCommon<T>::setMulScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline T Quat<T>::length() const
{
    return QuatCalcCommon<T>::length(*this);
}

template <typename T>
inline T Quat<T>::squaredLength() const
{
    return QuatCalcCommon<T>::squaredLength(*this);
}

template <typename T>
inline T Quat<T>::normalize()
{
    return QuatCalcCommon<T>::normalize(*this);
}

template <typename T>
inline T Quat<T>::dot(const Quat& q) const
{
    return QuatCalcCommon<T>::dot(*this, q);
}

template <typename T>
inline void Quat<T>::inverse()
{
    QuatCalcCommon<T>::setInverse(*this, *this);
}

template <typename T>
inline void Quat<T>::makeUnit()
{
    QuatCalcCommon<T>::makeUnit(*this);
}

template <typename T>
inline bool Quat<T>::makeVectorRotation(const Vec3& from, const Vec3& to)
{
    return QuatCalcCommon<T>::makeVectorRotation(*this, from, to);
}

template <typename T>
inline void Quat<T>::set(const Quat& other)
{
    QuatCalcCommon<T>::set(*this, other);
}

template <typename T>
inline void Quat<T>::set(T w_, T x_, T y_, T z_)
{
    QuatCalcCommon<T>::set(*this, w_, x_, y_, z_);
}

template <typename T>
inline void Quat<T>::setRPY(T roll, T pitch, T yaw)
{
    QuatCalcCommon<T>::setRPY(*this, roll, pitch, yaw);
}

template <typename T>
inline void Quat<T>::setAxisAngle(const Vec3& axis, T angle)
{
    QuatCalcCommon<T>::setAxisAngle(*this, axis, angle);
}

template <typename T>
inline void Quat<T>::setAxisRadian(const Vec3& axis, T radian)
{
    QuatCalcCommon<T>::setAxisRadian(*this, axis, radian);
}

template <typename T>
inline void Quat<T>::setAdd(const Quat& a, const Quat& b)
{
    QuatCalcCommon<T>::add(*this, a, b);
}

template <typename T>
inline void Quat<T>::setSub(const Quat& a, const Quat& b)
{
    QuatCalcCommon<T>::sub(*this, a, b);
}

template <typename T>
inline void Quat<T>::setMul(const Quat& a, const Quat& b)
{
    QuatCalcCommon<T>::setMul(*this, a, b);
}

template <typename T>
inline void Quat<T>::setInverse(const Quat& q)
{
    QuatCalcCommon<T>::setInverse(*this, q);
}

template <typename T>
inline void Quat<T>::calcRPY(Vec3& vec) const
{
    QuatCalcCommon<T>::calcRPY(vec, *this);
}

template <typename T>
inline void Quat<T>::slerpTo(const Quat& q1, const Quat& q2, f32 t)
{
    QuatCalcCommon<T>::slerpTo(*this, q1, q2, t);
}

}  // namespace sead
