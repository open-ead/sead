#include <gfx/seadDirectProjection.h>

namespace sead
{

DirectProjection::DirectProjection()
{
    setDirty();
}

DirectProjection::DirectProjection(const Matrix44f* mtx, Graphics::DevicePosture posture)
{
    setDirectProjectionMatrix(mtx, posture);
}

void DirectProjection::updateAttributesForDirectProjection()
{
    Matrix44f newMatrix;
    newMatrix.setInverse(mDirectMatrix);
}

void DirectProjection::doUpdateMatrix(Matrix44f* dst) const
{
    *dst = mDirectMatrix;
}

void DirectProjection::setDirectProjectionMatrix(const Matrix44f* mtx,
                                                 Graphics::DevicePosture posture)
{
    mDirectMatrix = (*mtx);
}

void DirectProjection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    Matrix44f inverseDirect;
    inverseDirect.setInverse(mDirectMatrix);
    f32 scale = 1.0f / (inverseDirect(3, 3) + screen_pos.x * inverseDirect(3, 0) +
                        screen_pos.y * inverseDirect(3, 1) + screen_pos.z * inverseDirect(3, 2));
    dst->x = scale * (inverseDirect(0, 3) + screen_pos.x * inverseDirect(0, 0) +
                      screen_pos.y * inverseDirect(0, 1) + screen_pos.z * inverseDirect(0, 2));
    dst->y = scale * (inverseDirect(1, 3) + screen_pos.x * inverseDirect(1, 0) +
                      screen_pos.y * inverseDirect(1, 1) + screen_pos.z * inverseDirect(1, 2));
    dst->z = scale * (inverseDirect(2, 3) + screen_pos.x * inverseDirect(2, 0) +
                      screen_pos.y * inverseDirect(2, 1) + screen_pos.z * inverseDirect(2, 2));
}
}  // namespace sead
