#ifndef SEAD_MATRIX34_H_
#define SEAD_MATRIX34_H_

#include <sead/seadPolicies.h>

namespace sead {

template <typename T>
struct Matrix34 : public Policies<T>::Mtx34Base
{
    Matrix34() { }

    Matrix34(
        T _00, T _01, T _02, T _03,
        T _10, T _11, T _12, T _13,
        T _20, T _21, T _22, T _23
    )
    {
        m[0][0] = _00;
        m[0][1] = _01;
        m[0][2] = _02;
        m[0][3] = _03;

        m[1][0] = _10;
        m[1][1] = _11;
        m[1][2] = _12;
        m[1][3] = _13;

        m[2][0] = _20;
        m[2][1] = _21;
        m[2][2] = _22;
        m[2][3] = _23;
    }

    static const Matrix34 zero;
    static const Matrix34 ident;
};

template <typename T>
struct Matrix44 : public Policies<T>::Mtx44Base
{
    Matrix44() { }

    Matrix44(
        T _00, T _01, T _02, T _03,
        T _10, T _11, T _12, T _13,
        T _20, T _21, T _22, T _23,
        T _30, T _31, T _32, T _33
    )
    {
        m[0][0] = _00;
        m[0][1] = _01;
        m[0][2] = _02;
        m[0][3] = _03;

        m[1][0] = _10;
        m[1][1] = _11;
        m[1][2] = _12;
        m[1][3] = _13;

        m[2][0] = _20;
        m[2][1] = _21;
        m[2][2] = _22;
        m[2][3] = _23;

        m[3][0] = _30;
        m[3][1] = _31;
        m[3][2] = _32;
        m[3][3] = _33;
    }

    static const Matrix44 zero;
    static const Matrix44 ident;
};

template <>
extern const Matrix34<f32> Matrix34<f32>::zero;

template <>
extern const Matrix34<f32> Matrix34<f32>::ident;

template <>
extern const Matrix44<f32> Matrix44<f32>::zero;

template <>
extern const Matrix44<f32> Matrix44<f32>::ident;

typedef Matrix34<f32> Matrix34f;
typedef Matrix44<f32> Matrix44f;

}  // namespace sead

#endif // SEAD_MATRIX34_H_
