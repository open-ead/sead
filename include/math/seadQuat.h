#pragma once

#include <basis/seadTypes.h>
#include <cmath>
#include <limits>
#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
struct Quat : public Policies<T>::QuatBase
{
    Quat() {}
    Quat(const Quat& other) { *this = other; }
    Quat(T x, T y, T z, T w) { set(x, y, z, w); }

    void set(T x, T y, T z, T w)
    {
        this->w = w;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    constexpr Quat& operator=(const Quat& other)
    {
        this->w = other.w;
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    constexpr friend Quat operator*(const Quat& a, T t)
    {
        auto result = a;
        result *= t;
        return result;
    }

    constexpr friend Quat operator*(const Quat& a, const Quat& b)
    {
        auto result = a;
        result *= b;
        return result;
    }

    constexpr friend Quat operator*(T t, const Quat& a) { return operator*(a, t); }

    constexpr Quat& operator*=(const Quat& t)
    {
        this->w *= t.w;
        this->x *= t.x;
        this->y *= t.y;
        this->z *= t.z;
        return *this;
    }

    constexpr Quat& operator*=(T t)
    {
        this->w *= t;
        this->x *= t;
        this->y *= t;
        this->z *= t;
        return *this;
    }

    void normalize();
    void invert(Quat* inverse);

    static const Quat unit;
};

using Quatf = Quat<f32>;

template <>
const Quatf Quatf::unit;

template <typename T>
constexpr T dot(const Quat<T>& u, const Quat<T>& v)
{
    return (u.w * v.w) + (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

template <typename T>
inline T norm2(const Quat<T>& v)
{
    return std::sqrt(dot(v, v));
}

template <typename T>
inline void Quat<T>::normalize()
{
    const auto norm = norm2(*this);
    if (norm > 0.0)
        *this *= T(1) / norm;
}

template <typename T>
inline void Quat<T>::invert(Quat* inverse)
{
    T prod = dot(*this, *this);
    if (prod > std::numeric_limits<T>::epsilon())
    {
        T inv = T(1) / prod;
        inverse->w = inv * this->w;
        inverse->x = inv * -this->x;
        inverse->y = inv * -this->y;
        inverse->z = inv * -this->z;
    }
    else
    {
        inverse->w = this->w;
        inverse->x = -this->x;
        inverse->y = -this->y;
        inverse->z = -this->z;
    }
}

}  // namespace sead
