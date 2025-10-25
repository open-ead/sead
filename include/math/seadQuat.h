#pragma once

#include <basis/seadTypes.h>
#include <cmath>
#include <limits>
#include <math/seadMathPolicies.h>
#include <math/seadVector.h>

namespace sead
{
template <typename T>
struct Quat : public Policies<T>::QuatBase
{
private:
    typedef Vector3<T> Vec3;

public:
    Quat() {}
    Quat(const Quat& other) = default;
    Quat(T w, T x, T y, T z);

    Quat& operator=(const Quat& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    Quat& operator+=(const Quat& other);
    friend Quat operator+(const Quat& a, const Quat& b)
    {
        Quat o;
        QuatCalcCommon<T>::add(o, a, b);
        return o;
    }

    Quat& operator-=(const Quat& other);
    friend Quat operator-(const Quat& a, const Quat& b)
    {
        Quat o;
        QuatCalcCommon<T>::sub(o, a, b);
        return o;
    }

    friend Quat operator*(const Quat& q, T t)
    {
        Quat o;
        QuatCalcCommon<T>::setMulScalar(o, q, t);
        return o;
    }

    friend Quat operator*(const Quat& a, const Quat& b)
    {
        Quat o;
        QuatCalcCommon<T>::setMul(o, a, b);
        return o;
    }

    friend Quat operator*(T t, const Quat& q) { return operator*(q, t); }

    Quat& operator*=(const Quat& t);

    Quat& operator*=(T t);

    bool operator==(const Quat& rhs) const
    {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
    }

    T length() const;
    T squaredLength() const;
    T normalize();
    T dot(const Quat& q) const;
    void inverse();

    void makeUnit();
    bool makeVectorRotation(const Vec3& from, const Vec3& to);
    void set(const Quat& other);
    void set(T w, T x, T y, T z);
    void setRPY(T roll, T pitch, T yaw);
    void setAxisAngle(const Vec3& axis, T angle);
    void setAxisRadian(const Vec3& axis, T radian);
    void setAdd(const Quat& a, const Quat& b);
    void setSub(const Quat& a, const Quat& b);
    void setMul(const Quat& a, const Quat& b);
    void setInverse(const Quat& q);
    void calcRPY(Vec3& rpy) const;
    void slerpTo(const Quat& q1, const Quat& q2, f32 t);

    static const Quat unit;
};

using Quatf = Quat<f32>;

template <>
const Quatf Quatf::unit;

}  // namespace sead

#define SEAD_MATH_QUAT_H_
#include <math/seadQuat.hpp>
#undef SEAD_MATH_QUAT_H_
