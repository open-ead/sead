#include <math/seadMatrix.h>

namespace sead
{
template <>
const Matrix22f Matrix22f::zero(0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix22f Matrix22f::ident(1.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix33f Matrix33f::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix33f Matrix33f::ident(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix34f Matrix34f::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f);

template <>
const Matrix34f Matrix34f::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                 0.0f);

template <>
const Matrix44f Matrix44f::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix44f Matrix44f::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix22d Matrix22d::zero(0.0, 0.0, 0.0, 0.0);

template <>
const Matrix22d Matrix22d::ident(1.0, 0.0, 0.0, 1.0);

template <>
const Matrix33d Matrix33d::zero(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

template <>
const Matrix33d Matrix33d::ident(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

template <>
const Matrix34d Matrix34d::zero(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

template <>
const Matrix34d Matrix34d::ident(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

template <>
const Matrix44d Matrix44d::zero(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                0.0, 0.0, 0.0);

template <>
const Matrix44d Matrix44d::ident(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                                 0.0, 0.0, 1.0);

}  // namespace sead
