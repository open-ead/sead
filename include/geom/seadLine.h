#pragma once

#include <math/seadVector.h>

namespace sead
{

template <typename VectorType>
class Segment
{
public:
    using T = typename VectorType::ValueType;

public:
    Segment() : mP0(VectorType::zero), mP1(VectorType::ex) {}
    Segment(const VectorType& p0, const VectorType& p1) : mP0(p0), mP1(p1) {}

    const VectorType& getPos0() const { return mP0; }
    void setPos0(const VectorType& p0) { mP0 = p0; }

    const VectorType& getPos1() const { return mP1; }
    void setPos1(const VectorType& p1) { mP1 = p1; }

private:
    VectorType mP0;
    VectorType mP1;
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
    T position;
    T direction;
};

using Ray2f = Ray<Vector2f>;
using Ray3f = Ray<Vector3f>;

}  // namespace sead
