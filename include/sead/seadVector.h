#ifndef SEAD_VECTOR_H_
#define SEAD_VECTOR_H_

#include "sead/seadPolicies.h"

namespace sead {

template <typename T>
struct Vector2 : public Policies<T>::Vec2Base
{
    Vector2() { }
    Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    static const Vector2 zero;
    static const Vector2 ex;
    static const Vector2 ey;
    static const Vector2 ones;
};

template <typename T>
struct Vector3 : public Policies<T>::Vec3Base
{
    Vector3() { }
    Vector3(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    static const Vector3 zero;
    static const Vector3 ex;
    static const Vector3 ey;
    static const Vector3 ez;
    static const Vector3 ones;
};

template <typename T>
struct Vector4 : public Policies<T>::Vec4Base
{
    Vector4() { }
    Vector4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    static const Vector4 zero;
    static const Vector4 ex;
    static const Vector4 ey;
    static const Vector4 ez;
    static const Vector4 ew;
    static const Vector4 ones;
};

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector4<f32> Vector4f;

template <>
extern const Vector2<f32> Vector2<f32>::zero;

template <>
extern const Vector2<f32> Vector2<f32>::ex;

template <>
extern const Vector2<f32> Vector2<f32>::ey;

template <>
extern const Vector2<f32> Vector2<f32>::ones;

template <>
extern const Vector3<f32> Vector3<f32>::zero;

template <>
extern const Vector3<f32> Vector3<f32>::ex;

template <>
extern const Vector3<f32> Vector3<f32>::ey;

template <>
extern const Vector3<f32> Vector3<f32>::ez;

template <>
extern const Vector3<f32> Vector3<f32>::ones;

template <>
extern const Vector4<f32> Vector4<f32>::zero;

template <>
extern const Vector4<f32> Vector4<f32>::ex;

template <>
extern const Vector4<f32> Vector4<f32>::ey;

template <>
extern const Vector4<f32> Vector4<f32>::ez;

template <>
extern const Vector4<f32> Vector4<f32>::ew;

template <>
extern const Vector4<f32> Vector4<f32>::ones;

}  // namespace sead

#endif // #define SEAD_VECTOR_H_
