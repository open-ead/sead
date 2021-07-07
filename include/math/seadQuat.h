#pragma once

#include <basis/seadTypes.h>
#include <cmath>
#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
struct Quat : public Policies<T>::QuatBase
{
    Quat() {}
    Quat(const Quat& other) = default;
    Quat(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    constexpr Quat& operator=(const Quat& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    constexpr friend Quat operator*(const Quat& a, T t)
    {
        return {a.x * t, a.y * t, a.z * t, a.w * t};
    }
    constexpr friend Quat operator*(const Quat& a, const Quat& b)
    {
        return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    }
    constexpr friend Quat operator*(T t, const Quat& a) { return operator*(a, t); }

    constexpr Quat& operator*=(T t)
    {
        this->w *= t;
        this->x *= t;
        this->y *= t;
        this->z *= t;
        return *this;
    }

    static const Quat unit;
    void normalize();
    Quat inverse();
};

typedef Quat<f32> Quatf;

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

//NON_MATCHING: Order of parts unknown. Used in `physSupportBoneResource.cpp` - SupportBoneResource::postRead_
template <typename T>
inline void Quat<T>::normalize()
{
    const auto norm = norm2(*this);
    if(norm > 0.0){
        T fac = T(1) / norm;
        this->w *= fac;
        this->z *= fac;
        this->x *= fac;
        this->y *= fac;
    }
}

template <typename T>
inline Quat<T> Quat<T>::inverse()
{
    auto inverse = *this;
    T prod = dot(*this, *this);
    if (prod > std::numeric_limits<T>::epsilon())
        inverse *= T(1) / prod;
    return {-inverse.x, -inverse.y, -inverse.z, inverse.w};
}

}  // namespace sead
