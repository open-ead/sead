#include <gfx/seadDirectProjection.h>

namespace sead
{

DirectProjection::DirectProjection()
{
    mDirectMatrix.makeIdentity();
    mNear = 0.0;
    mFar = 0.0;
    mFovy = 0.0;
    mAspect = 0.0;
    mOffset.set(0.0, 0.0);
    mUnknown7 = 1;
    markDirty();
    markDeviceDirty();
}

DirectProjection::DirectProjection(Matrix44f const& mtx, Graphics::DevicePosture posture)
{
    setDirectProjectionMatrix(mtx, posture);
}

void DirectProjection::setDirectProjectionMatrix(const Matrix44f& mtx,
                                                 Graphics::DevicePosture posture)
{
    mDirectMatrix = mtx;
    switch (posture)
    {
    case Graphics::DevicePosture::cDevicePosture_RotateRight:
        break;
    case Graphics::DevicePosture::cDevicePosture_RotateLeft:
        break;
    case Graphics::DevicePosture::cDevicePosture_RotateHalfAround:
        break;
    case Graphics::DevicePosture::cDevicePosture_FlipY:
        break;
    }
    markDirty();
}

float DirectProjection::getNear() const
{
    return mNear;
}

float DirectProjection::getFar() const
{
    return mFar;
}

float DirectProjection::getFovy() const
{
    return mFovy;
}

float DirectProjection::getAspect() const
{
    return mAspect;
}

void DirectProjection::getOffset(sead::Vector2<float>* offset) const
{
    offset->set(mOffset);
}

Projection::ProjectionType DirectProjection::getProjectionType() const
{
    return Projection::ProjectionType::cDirectProjection;
}
}  // namespace sead
