#include <gfx/seadFrustumProjection.h>

namespace sead
{
FrustumProjection::FrustumProjection(float near, float far, float top, float bottom, float left,
                                     float right)
{
    mNear = near;
    mFar = far;
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    markDirty();
}

float FrustumProjection::getNear() const
{
    return mNear;
}

float FrustumProjection::getFar() const
{
    return mFar;
}

void FrustumProjection::doScreenPosToCameraPosTo(Vector3f* screenPos,
                                                 const Vector3f& cameraPos) const
{
    screenPos->z = mNear;
    screenPos->x = (float)0.5 * ((mRight + mLeft) + (mRight - mLeft) * cameraPos.x);
    screenPos->y = (float)0.5 * ((mTop + mBottom) + (mTop - mBottom) * cameraPos.y);
}

float FrustumProjection::getFovy() const
{
    return (float)2.0 * atan2f((float)0.5 * (mTop - mBottom), getNear());
}

float FrustumProjection::getAspect() const
{
    return (mRight - mLeft) / (mTop - mBottom);
}

void FrustumProjection::getOffset(Vector2f* offset) const
{
    float denom = mRight - mLeft;
    offset->x = (float)0.5 * (mRight + mLeft) / denom;

    denom = mTop - mBottom;
    offset->y = (float)0.5 * (mTop + mBottom) / denom;
}

Projection::ProjectionType FrustumProjection::getProjectionType() const
{
    return ProjectionType::cPerspectiveProjection;
}
}  // namespace sead
