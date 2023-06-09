#include <gfx/seadDirectProjection.h>

namespace sead
{

DirectProjection::DirectProjection(Matrix44f const& mtx, Graphics::DevicePosture posture)
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

void DirectProjection::setDirectProjectionMatrix(const Matrix44f& mtx,
                                                 Graphics::DevicePosture posture)
{
    mDirectMatrix = mtx;
    markDirty();
}

float DirectProjection::getNear() const
{
    return (float)0.0;
}
float DirectProjection::getFar() const
{
    return (float)0.0;
}
float DirectProjection::getFovy() const
{
    return (float)0.0;
}
float DirectProjection::getAspect() const
{
    return (float)0.0;
}
// void DirectProjection::getOffset(sead::Vector2<float>* offset) const {}

Projection::ProjectionType DirectProjection::getProjectionType() const
{
    return Projection::ProjectionType::cDirectProjection;
}
}  // namespace sead
