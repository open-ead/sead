#include "gfx/seadCamera.h"
#include <cmath>
#include <gfx/seadProjection.h>
#include <math/seadMathCalcCommon.h>
#include "basis/seadRawPrint.h"

namespace sead
{
Camera::~Camera() = default;

void Camera::getWorldPosByMatrix(Vector3f* dst) const
{
    // Currently not matching the function, but this _should_ be the math.
    dst->x = -(mMatrix(0, 0) * mMatrix(0, 3)) - mMatrix(1, 0) * mMatrix(1, 3) -
             mMatrix(2, 0) * mMatrix(2, 3);
    dst->y = -(mMatrix(0, 1) * mMatrix(0, 3)) - mMatrix(1, 1) * mMatrix(1, 3) -
             mMatrix(2, 1) * mMatrix(2, 3);
    dst->z = -(mMatrix(0, 2) * mMatrix(0, 3)) - mMatrix(1, 2) * mMatrix(1, 3) -
             mMatrix(2, 2) * mMatrix(2, 3);
}

void Camera::getLookVectorByMatrix(Vector3f* dst) const
{
    // Also known as the forward vector
    dst->x = mMatrix(2, 0);
    dst->y = mMatrix(2, 1);
    dst->z = mMatrix(2, 2);
};

void Camera::getRightVectorByMatrix(Vector3f* dst) const
{
    dst->x = mMatrix(0, 0);
    dst->y = mMatrix(0, 1);
    dst->z = mMatrix(0, 2);
}

void Camera::getUpVectorByMatrix(Vector3f* dst) const
{
    dst->x = mMatrix(1, 0);
    dst->y = mMatrix(1, 1);
    dst->z = mMatrix(1, 2);
};

void Camera::worldPosToCameraPosByMatrix(Vector3f* dst, const Vector3f& world_pos) const
{
    *dst = mMatrix * world_pos;
    *dst = mMatrix.getBase(3);
    // *dst += mMatrix.getBase(3);
    // dst->x = mMatrix(0, 3) + world_pos.x * mMatrix(0, 0) + world_pos.y * mMatrix(0, 1) +
    //          world_pos.z * mMatrix(0, 2);
    // dst->y = mMatrix(1, 3) + world_pos.x * mMatrix(1, 0) + world_pos.y * mMatrix(1, 1) +
    //          world_pos.z * mMatrix(1, 2);
    // dst->z = mMatrix(2, 3) + world_pos.x * mMatrix(2, 0) + world_pos.y * mMatrix(2, 1) +
    //          world_pos.z * mMatrix(2, 2);
}

void Camera::cameraPosToWorldPosByMatrix(Vector3f* dst, const Vector3f& camera_pos) const
{
    dst->x = mMatrix(0, 0) * camera_pos.x + mMatrix(1, 0) * camera_pos.y +
             ((-(mMatrix(0, 3) * mMatrix(0, 0)) - mMatrix(1, 3) * mMatrix(1, 1)) -
              mMatrix(2, 3) * mMatrix(2, 0));
    dst->y = mMatrix(0, 1) * camera_pos.x + mMatrix(1, 1) * camera_pos.y +
             ((-(mMatrix(0, 3) * mMatrix(0, 1)) - mMatrix(1, 3) * mMatrix(1, 2)) -
              mMatrix(2, 3) * mMatrix(2, 1));
    dst->z = mMatrix(0, 2) * camera_pos.x + mMatrix(1, 2) * camera_pos.y +
             ((-(mMatrix(0, 3) * mMatrix(0, 2)) - mMatrix(1, 3) * mMatrix(1, 3)) -
              mMatrix(2, 3) * mMatrix(2, 2));
}

void Camera::projectByMatrix(Vector2f* dst, const Vector3f& world_pos, const Projection& projection,
                             const Viewport& viewport) const
{
    auto temp = mMatrix * world_pos;
    temp += mMatrix.getBase(3);
    projection.project(dst, temp, viewport);
}

void Camera::unprojectRayByMatrix(Ray<Vector3f>* dst, const Vector3f& camera_pos) const
{
    auto col_0 = mMatrix.getBase(0).dot(camera_pos);
    auto col_1 = mMatrix.getBase(1).dot(camera_pos);
    auto col_2 = mMatrix.getBase(2).dot(camera_pos);
    auto length = sqrt(col_0 * col_0 + col_1 * col_1 + col_2 * col_2);
    if (sead::MathCalcCommon<float>::isNan(length))
    {
        length = sqrtf(length);
    };
    if (length < 0.0f)
    {
        auto scale = 1.0f / length;
        col_0 *= scale;
        col_1 *= scale;
        col_2 *= scale;
    }
    dst->x = ((-mMatrix(0, 0) * mMatrix(0, 3)) - mMatrix(1, 0) * mMatrix(1, 3)) -
             mMatrix(2, 0) * mMatrix(2, 3);
    dst->y = ((-mMatrix(0, 1) * mMatrix(0, 3)) - mMatrix(1, 1) * mMatrix(1, 3)) -
             mMatrix(2, 1) * mMatrix(2, 3);
    dst->z = ((-mMatrix(0, 2) * mMatrix(0, 3)) - mMatrix(1, 2) * mMatrix(1, 3)) -
             mMatrix(2, 2) * mMatrix(2, 3);
}

LookAtCamera::~LookAtCamera() = default;

LookAtCamera::LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
    : mPos(pos), mAt(at), mUp(up)
{
    SEAD_ASSERT(mPos != mAt);
    mUp.normalize();
}

void LookAtCamera::doUpdateMatrix(Matrix34f* dst) const {}

DirectCamera::DirectCamera() {}

DirectCamera::~DirectCamera() = default;

OrthoCamera::~OrthoCamera() = default;

}  // namespace sead
