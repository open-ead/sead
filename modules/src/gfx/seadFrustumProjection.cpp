#include <gfx/seadFrustumProjection.h>
#include "math/seadVectorFwd.h"

namespace sead
{
FrustumProjection::FrustumProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right)
    : mNear(_near), mFar(_far), mTop(top), mBottom(bottom), mLeft(left), mRight(right)
{
    setDirty();
}

FrustumProjection::FrustumProjection(f32 _near, f32 _far, const BoundBox2f& box)
    : mNear(_near), mFar(_far)
{
}

void FrustumProjection::doUpdateMatrix(Matrix44f* dst) const
{
    f32 inv_size = 1.0f / (mRight - mLeft);

    (*dst)(0, 0) = mNear * 2 * inv_size;
    (*dst)(0, 1) = 0.0f;
    (*dst)(0, 2) = (mLeft + mRight) * inv_size;
    (*dst)(0, 3) = 0.0f;

    inv_size = 1.0f / (mTop - mBottom);

    (*dst)(1, 0) = 0.0f;
    (*dst)(1, 1) = mNear * 2 * inv_size;
    (*dst)(1, 2) = (mTop + mBottom) * inv_size;
    (*dst)(1, 3) = 0.0f;

    inv_size = 1.0f / (mFar - mNear);

    (*dst)(2, 0) = 0.0f;
    (*dst)(2, 1) = 0.0f;
    (*dst)(2, 2) = -(mFar + mNear) * inv_size;
    (*dst)(2, 3) = -(mFar * 2 * mNear) * inv_size;

    (*dst)(3, 0) = 0.0f;
    (*dst)(3, 1) = 0.0f;
    (*dst)(3, 2) = -1.0f;
    (*dst)(3, 3) = 0.0f;
}

void FrustumProjection::setNear(f32 near)
{
    mNear = near;
    setDirty();
}

void FrustumProjection::setFar(f32 far)
{
    mFar = far;
    setDirty();
}

void FrustumProjection::setTop(f32 top)
{
    mTop = top;
    setDirty();
}

void FrustumProjection::setBottom(f32 bottom)
{
    mBottom = bottom;
    setDirty();
}

void FrustumProjection::setLeft(f32 left)
{
    mLeft = left;
    setDirty();
}

void FrustumProjection::setRight(f32 right)
{
    mRight = right;
    setDirty();
}

f32 FrustumProjection::getFovy() const
{
    return 2 * Mathf::atan2(0.5f * (mTop - mBottom), getNear());
}

f32 FrustumProjection::getAspect() const
{
    return (mRight - mLeft) / (mTop - mBottom);
}

void FrustumProjection::getOffset(Vector2f* dst) const
{
    float denom = mRight - mLeft;
    dst->x = (float)0.5 * (mRight + mLeft) / denom;

    denom = mTop - mBottom;
    dst->y = (float)0.5 * (mTop + mBottom) / denom;
}

}  // namespace sead
