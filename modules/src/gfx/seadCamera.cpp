#include "gfx/seadCamera.h"
#include <gfx/seadProjection.h>
#include <math/seadMathCalcCommon.h>
#include "basis/seadRawPrint.h"

namespace sead
{
Camera::~Camera() = default;

void Camera::getWorldPosByMatrix(Vector3f* dst) const
{
    dst->x = mMatrix.getBase(3).dot(mMatrix.getBase(0));
    dst->x = mMatrix.getBase(3).dot(mMatrix.getBase(1));
    dst->x = mMatrix.getBase(3).dot(mMatrix.getBase(2));
    *dst *= -1.0f;
}

void Camera::getLookVectorByMatrix(Vector3f* dst) const
{
    // Also known as the forward vector
    auto vec = mMatrix.getRow(2);
    dst->set(vec.x, vec.y, vec.z);
};

void Camera::getRightVectorByMatrix(Vector3f* dst) const
{
    auto vec = mMatrix.getRow(0);
    dst->set(vec.x, vec.y, vec.z);
}

void Camera::getUpVectorByMatrix(Vector3f* dst) const
{
    auto vec = mMatrix.getRow(1);
    dst->set(vec.x, vec.y, vec.z);
};

void Camera::worldPosToCameraPosByMatrix(Vector3f* dst, const Vector3f& world_pos) const
{
    *dst = mMatrix * world_pos;
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
    dst->direction.x = mMatrix.getBase(0).dot(camera_pos);
    dst->direction.y = mMatrix.getBase(1).dot(camera_pos);
    dst->direction.z = mMatrix.getBase(2).dot(camera_pos);
    dst->direction.normalize();

    dst->position.x = ((-mMatrix(0, 0) * mMatrix(0, 3)) - mMatrix(1, 0) * mMatrix(1, 3)) -
                      mMatrix(2, 0) * mMatrix(2, 3);
    dst->position.y = ((-mMatrix(0, 1) * mMatrix(0, 3)) - mMatrix(1, 1) * mMatrix(1, 3)) -
                      mMatrix(2, 1) * mMatrix(2, 3);
    dst->position.z = ((-mMatrix(0, 2) * mMatrix(0, 3)) - mMatrix(1, 2) * mMatrix(1, 3)) -
                      mMatrix(2, 2) * mMatrix(2, 3);
    dst->position *= -1.0f;
}

LookAtCamera::~LookAtCamera() = default;

LookAtCamera::LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
    : mPos(pos), mAt(at), mUp(up)
{
    SEAD_ASSERT(mPos != mAt);
    mUp.normalize();
}

void LookAtCamera::doUpdateMatrix(Matrix34f* dst) const {}

DirectCamera::~DirectCamera() = default;

OrthoCamera::OrthoCamera() = default;

OrthoCamera::~OrthoCamera() = default;

}  // namespace sead
