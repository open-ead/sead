#pragma once

#include <math/seadVector.h>

namespace sead
{

template <typename T>
class Segment
{
public:
    Segment() = default;
    Segment(const T& p0, const T& p1) : mP0(p0), mP1(p1) {}

    const T& getPos0() const { return mP0; }
    void setPos0(const T& p0) { mP0 = p0; }

    const T& getPos1() const { return mP1; }
    void setPos1(const T& p1) { mP1 = p1; }

private:
    T mP0 = T::zero;
    T mP1 = T::ex;
};

using Segment2f = Segment<Vector2f>;
using Segment3f = Segment<Vector3f>;

#ifdef cafe
static_assert(sizeof(Segment2f) == 0x10, "sead::Segment<T> size mismatch");
static_assert(sizeof(Segment3f) == 0x18, "sead::Segment<T> size mismatch");
#endif  // cafe

template <typename T>
class Ray
{
public:
    T mP;
    T mD;
};

using Ray2f = Ray<Vector2f>;
using Ray3f = Ray<Vector3f>;

}  // namespace sead
