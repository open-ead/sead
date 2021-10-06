#pragma once

#include <math/seadVectorCalcCommon.h>
#ifndef SEAD_MATH_VECTOR_H_
#include <math/seadVector.h>
#endif

namespace sead
{
template <typename T>
inline Vector2<T>::Vector2(T x_, T y_)
{
    Vector2CalcCommon<T>::set(*this, x_, y_);
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
    Vector2<T> o;
    Vector2CalcCommon<T>::add(o, a, b);
    return o;
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
    Vector2<T> o;
    Vector2CalcCommon<T>::sub(o, a, b);
    return o;
}

template <typename T>
inline Vector2<T> operator*(T t, const Vector2<T>& a)
{
    return operator*(a, t);
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T>& a, T t)
{
    return {a.x / t, a.y / t};
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
    Vector2CalcCommon<T>::add(*this, *this, other);
    return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
    Vector2CalcCommon<T>::sub(*this, *this, other);
    return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator*=(T t)
{
    this->x *= t;
    this->y *= t;
    return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator/=(T t)
{
    this->x /= t;
    this->y /= t;
    return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other)
{
    Vector2CalcCommon<T>::set(*this, other);
    return *this;
}

template <typename T>
inline void Vector2<T>::set(const Vector2<T>& other)
{
    Vector2CalcCommon<T>::set(*this, other);
}

template <typename T>
inline void Vector2<T>::set(T x_, T y_)
{
    Vector2CalcCommon<T>::set(*this, x_, y_);
}

template <typename T>
inline Vector3<T>::Vector3(T x_, T y_, T z_)
{
    Vector3CalcCommon<T>::set(*this, x_, y_, z_);
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
{
    Vector3<T> o;
    Vector3CalcCommon<T>::add(o, a, b);
    return o;
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& a, T t)
{
    return {a.x / t, a.y / t, a.z / t};
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other)
{
    Vector3CalcCommon<T>::add(*this, *this, other);
    return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& other)
{
    Vector3CalcCommon<T>::sub(*this, *this, other);
    return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator*=(T t)
{
    Vector3CalcCommon<T>::multScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator/=(T t)
{
    this->x /= t;
    this->y /= t;
    this->z /= t;
    return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator=(const Vector3<T>& other)
{
    Vector3CalcCommon<T>::set(*this, other);
    return *this;
}

template <typename T>
inline T Vector3<T>::dot(const Vector3<T>& t) const
{
    return Vector3CalcCommon<T>::dot(*this, t);
}

template <typename T>
inline T Vector3<T>::length() const
{
    return Vector3CalcCommon<T>::length(*this);
}

template <typename T>
inline void Vector3<T>::add(const Vector3<T>& a)
{
    Vector3CalcCommon<T>::add(*this, *this, a);
}

template <typename T>
inline void Vector3<T>::multScalar(T t)
{
    Vector3CalcCommon<T>::multScalar(*this, *this, t);
}

template <typename T>
inline T Vector3<T>::normalize()
{
    return Vector3CalcCommon<T>::normalize(*this);
}

template <typename T>
inline void Vector3<T>::set(const Vector3<T>& other)
{
    Vector3CalcCommon<T>::set(*this, other);
}

template <typename T>
inline void Vector3<T>::set(T x_, T y_, T z_)
{
    Vector3CalcCommon<T>::set(*this, x_, y_, z_);
}

template <typename T>
inline void Vector3<T>::setCross(const Vector3<T>& a, const Vector3<T>& b)
{
    Vector3CalcCommon<T>::cross(*this, a, b);
}

template <typename T>
inline void Vector3<T>::setScaleAdd(T t, const Vector3<T>& a, const Vector3<T>& b)
{
    Vector3CalcCommon<T>::multScalarAdd(*this, t, a, b);
}

template <typename T>
inline Vector4<T>::Vector4(T x_, T y_, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, x_, y_, z_, w_);
}

template <typename T>
inline Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

template <typename T>
inline Vector4<T> operator*(T t, const Vector4<T>& a)
{
    return operator*(a, t);
}

template <typename T>
inline Vector4<T> operator/(const Vector4<T>& a, T t)
{
    return {a.x / t, a.y / t, a.z / t, a.w / t};
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->w += other.w;
    return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->w -= other.w;
    return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator*=(T t)
{
    this->x *= t;
    this->y *= t;
    this->z *= t;
    this->w *= t;
    return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator/=(T t)
{
    this->x /= t;
    this->y /= t;
    this->z /= t;
    this->w /= t;
    return *this;
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator=(const Vector4<T>& other)
{
    Vector4CalcCommon<T>::set(*this, other);
    return *this;
}

template <typename T>
inline void Vector4<T>::set(const Vector4<T>& other)
{
    Vector4CalcCommon<T>::set(*this, other);
}

template <typename T>
inline void Vector4<T>::set(T x_, T y_, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, x_, y_, z_, w_);
}

}  // namespace sead
