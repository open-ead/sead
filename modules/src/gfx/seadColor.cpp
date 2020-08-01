#include <cmath>

#include <gfx/seadColor.h>
#include <math/seadMathCalcCommon.h>

namespace sead
{
const Color4f Color4f::cBlack(0.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cGray(0.5f, 0.5f, 0.5f, 1.0f);
const Color4f Color4f::cWhite(1.0f, 1.0f, 1.0f, 1.0f);
const Color4f Color4f::cRed(1.0f, 0.0f, 0.0f, 1.0f);
const Color4f Color4f::cGreen(0.0f, 1.0f, 0.0f, 1.0f);
const Color4f Color4f::cBlue(0.0f, 0.0f, 1.0f, 1.0f);
const Color4f Color4f::cYellow(1.0f, 1.0f, 0.0f, 1.0f);
const Color4f Color4f::cMagenta(1.0f, 0.0f, 1.0f, 1.0f);
const Color4f Color4f::cCyan(0.0f, 1.0f, 1.0f, 1.0f);
const f32 Color4f::cElementMax = 1.0f;
const f32 Color4f::cElementMin = 0.0f;

Color4f Color4f::lerp(const Color4f& color1, const Color4f& color2, f32 t)
{
    t = sead::clamp(t, cElementMin, cElementMax);
    const f32 a = sead::lerp(color1.a, color2.a, t);
    const f32 r = sead::lerp(color1.r, color2.r, t);
    const f32 g = sead::lerp(color1.g, color2.g, t);
    const f32 b = sead::lerp(color1.b, color2.b, t);
    return {r, g, b, a};
}

void Color4f::setLerp(const Color4f& color1, const Color4f& color2, f32 t)
{
    t = sead::clamp(t, cElementMin, cElementMax);
    a = sead::lerp(color1.a, color2.a, t);
    r = sead::lerp(color1.r, color2.r, t);
    g = sead::lerp(color1.g, color2.g, t);
    b = sead::lerp(color1.b, color2.b, t);
}

void Color4f::setGammaCollection(const Color4f& value, f32 gamma)
{
    a = value.a;
    r = std::powf(value.r, gamma);
    g = std::powf(value.g, gamma);
    b = std::powf(value.b, gamma);
}

void Color4f::adjustOverflow()
{
    r = sead::clamp(r, cElementMin, cElementMax);
    g = sead::clamp(g, cElementMin, cElementMax);
    b = sead::clamp(b, cElementMin, cElementMax);
    a = sead::clamp(a, cElementMin, cElementMax);
}

#define SEAD_COLOR4F_OPERATORS(OP, OP2)                                                            \
    Color4f& Color4f::operator OP(const Color4f& rhs)                                              \
    {                                                                                              \
        r OP rhs.r;                                                                                \
        g OP rhs.g;                                                                                \
        b OP rhs.b;                                                                                \
        a OP rhs.a;                                                                                \
        return *this;                                                                              \
    }                                                                                              \
    Color4f& Color4f::operator OP(f32 x)                                                           \
    {                                                                                              \
        r OP x;                                                                                    \
        g OP x;                                                                                    \
        b OP x;                                                                                    \
        a OP x;                                                                                    \
        return *this;                                                                              \
    }                                                                                              \
    Color4f operator OP2(const Color4f& lhs, const Color4f& rhs)                                   \
    {                                                                                              \
        Color4f result = lhs;                                                                      \
        result OP rhs;                                                                             \
        return result;                                                                             \
    }                                                                                              \
    Color4f operator OP2(const Color4f& lhs, f32 x)                                                \
    {                                                                                              \
        Color4f result = lhs;                                                                      \
        result OP x;                                                                               \
        return result;                                                                             \
    }

SEAD_COLOR4F_OPERATORS(+=, +)
SEAD_COLOR4F_OPERATORS(-=, -)
SEAD_COLOR4F_OPERATORS(*=, *)
SEAD_COLOR4F_OPERATORS(/=, /)

bool operator==(const Color4f& lhs, const Color4f& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

}  // namespace sead
