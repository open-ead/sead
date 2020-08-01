/*
 *  Copyright (c) 2020 The Starlight Project
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <array>

#include <basis/seadTypes.h>

namespace sead
{
class Color4f
{
public:
    Color4f() = default;
    Color4f(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}

    static Color4f lerp(const Color4f& color1, const Color4f& color2, f32 t);

    void setLerp(const Color4f& color1, const Color4f& color2, f32 t);
    // This should actually be called setGammaCorrection...
    void setGammaCollection(const Color4f& value, f32 gamma);
    void adjustOverflow();

    Color4f& operator+=(const Color4f& rhs);
    Color4f& operator-=(const Color4f& rhs);
    Color4f& operator*=(const Color4f& rhs);
    Color4f& operator/=(const Color4f& rhs);
    Color4f& operator+=(f32 x);
    Color4f& operator-=(f32 x);
    Color4f& operator*=(f32 x);
    Color4f& operator/=(f32 x);

    friend Color4f operator+(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator-(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator*(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator/(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator+(const Color4f& lhs, f32 x);
    friend Color4f operator-(const Color4f& lhs, f32 x);
    friend Color4f operator*(const Color4f& lhs, f32 x);
    friend Color4f operator/(const Color4f& lhs, f32 x);
    friend bool operator==(const Color4f& lhs, const Color4f& rhs);

    f32 r;
    f32 g;
    f32 b;
    f32 a;

    static const f32 cElementMax;
    static const f32 cElementMin;

    static const Color4f cBlack;
    static const Color4f cGray;
    static const Color4f cWhite;
    static const Color4f cRed;
    static const Color4f cGreen;
    static const Color4f cBlue;
    static const Color4f cYellow;
    static const Color4f cMagenta;
    static const Color4f cCyan;
};
}  // namespace sead
