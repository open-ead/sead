#include <math/seadMatrix.h>

namespace sead
{
template <>
const Matrix22<f32> Matrix22<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix22<f32> Matrix22<f32>::ident(1.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix33<f32> Matrix33<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix33<f32> Matrix33<f32>::ident(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix34<f32> Matrix34<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f);

template <>
const Matrix34<f32> Matrix34<f32>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f);

template <>
const Matrix44<f32> Matrix44<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix44<f32> Matrix44<f32>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

template <>
const Matrix22<f64> Matrix22<f64>::zero(0, 0, 0, 0);

template <>
const Matrix22<f64> Matrix22<f64>::ident(1, 0, 0, 1);

template <>
const Matrix33<f64> Matrix33<f64>::zero(0, 0, 0, 0, 0, 0, 0, 0, 0);

template <>
const Matrix33<f64> Matrix33<f64>::ident(1, 0, 0, 0, 1, 0, 0, 0, 1);

template <>
const Matrix34<f64> Matrix34<f64>::zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

template <>
const Matrix34<f64> Matrix34<f64>::ident(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

template <>
const Matrix44<f64> Matrix44<f64>::zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

template <>
const Matrix44<f64> Matrix44<f64>::ident(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

template <>
void Matrix34CalcCommon<f32>::toQuat(BaseQuat<f32>& result, const BaseMtx34<f32>& matrix)
{
    const f32 w2 =
        (matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] + 1.0f) * 0.25f;
    const f32 x2 = w2 - (matrix.m[1][1] + matrix.m[2][2]) * 0.5f;
    const f32 y2 = w2 - (matrix.m[2][2] + matrix.m[0][0]) * 0.5f;
    const f32 z2 = w2 - (matrix.m[0][0] + matrix.m[1][1]) * 0.5f;

    const u8 index =
        w2 > x2
            ? (w2 > y2 ? (w2 > z2 ? 0 : 3) : (y2 > z2 ? 2 : 3))
            : (x2 > y2 ? (x2 > z2 ? 1 : 3) : (y2 > z2 ? 2 : 3));

    f32 last;
    f32 scale;
    switch (index & 3)
    {
    case 3:
    {
        const f32 root = MathCalcCommon<f32>::sqrt(z2);
        result.z = root;
        const f32 scale3 = 0.25f / root;
        result.w = scale3 * (matrix.m[1][0] - matrix.m[0][1]);
        result.x = scale3 * (matrix.m[2][0] + matrix.m[0][2]);
        result.y = scale3 * (matrix.m[2][1] + matrix.m[1][2]);
        return;
    }
    case 1:
    {
        const f32 root = MathCalcCommon<f32>::sqrt(x2);
        result.x = root;
        scale = 0.25f / root;
        result.w = scale * (matrix.m[2][1] - matrix.m[1][2]);
        result.y = scale * (matrix.m[0][1] + matrix.m[1][0]);
        last = matrix.m[0][2] + matrix.m[2][0];
        break;
    }
    case 2:
    {
        const f32 root = MathCalcCommon<f32>::sqrt(y2);
        result.y = root;
        const f32 scale2 = 0.25f / root;
        result.w = scale2 * (matrix.m[0][2] - matrix.m[2][0]);
        result.z = scale2 * (matrix.m[1][2] + matrix.m[2][1]);
        result.x = scale2 * (matrix.m[1][0] + matrix.m[0][1]);
        return;
    }
    default:
    {
        const f32 root = MathCalcCommon<f32>::sqrt(w2);
        scale = 0.25f / root;
        result.w = root;
        result.x = scale * (matrix.m[2][1] - matrix.m[1][2]);
        result.y = scale * (matrix.m[0][2] - matrix.m[2][0]);
        last = matrix.m[1][0] - matrix.m[0][1];
        break;
    }
    }

    result.z = scale * last;
}

}  // namespace sead
