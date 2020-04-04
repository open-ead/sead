#ifndef SEAD_MATRIX34_H_
#define SEAD_MATRIX34_H_

#include "sead/seadPolicies.h"

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
        rows[0][0] = _00;
        rows[0][1] = _01;
        rows[0][2] = _02;
        rows[0][3] = _03;

        rows[1][0] = _10;
        rows[1][1] = _11;
        rows[1][2] = _12;
        rows[1][3] = _13;

        rows[2][0] = _20;
        rows[2][1] = _21;
        rows[2][2] = _22;
        rows[2][3] = _23;
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
        rows[0][0] = _00;
        rows[0][1] = _01;
        rows[0][2] = _02;
        rows[0][3] = _03;

        rows[1][0] = _10;
        rows[1][1] = _11;
        rows[1][2] = _12;
        rows[1][3] = _13;

        rows[2][0] = _20;
        rows[2][1] = _21;
        rows[2][2] = _22;
        rows[2][3] = _23;

        rows[3][0] = _30;
        rows[3][1] = _31;
        rows[3][2] = _32;
        rows[3][3] = _33;
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
