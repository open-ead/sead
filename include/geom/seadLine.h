#ifndef SEAD_LINE_H_
#define SEAD_LINE_H_

#include <math/seadVector.h>

namespace sead {

template <typename VectorType>
class Segment
{
public:
    typedef typename VectorType::ValueType T;

public:
    Segment()
        : mP0(VectorType::zero)
        , mP1(VectorType::ex)
    {
    }

    Segment(const VectorType& p0, const VectorType& p1)
        : mP0(p0)
        , mP1(p1)
    {
    }

    const VectorType& getPos0() const
    {
        return mP0;
    }

    void setPos0(const VectorType& p0)
    {
        mP0 = p0;
    }

    const VectorType& getPos1() const
    {
        return mP1;
    }

    void setPos1(const VectorType& p1)
    {
        mP1 = p1;
    }

private:
    VectorType mP0;
    VectorType mP1;
};

typedef Segment<Vector2f> Segment2f;
typedef Segment<Vector3f> Segment3f;

#ifdef cafe
static_assert(sizeof(Segment2f) == 0x10, "sead::Segment<T> size mismatch");
static_assert(sizeof(Segment3f) == 0x18, "sead::Segment<T> size mismatch");
#endif // cafe

}  // namespace sead

#endif // #define SEAD_LINE_H_
