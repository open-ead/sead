#include "gfx/seadCamera.h"
#include <gfx/seadProjection.h>
#include <math/seadMathCalcCommon.h>
#include "basis/seadRawPrint.h"
#include "math/seadMatrixCalcCommon.h"

namespace sead
{

Camera::~Camera() = default;

void Camera::getWorldPosByMatrix(Vector3f* dst) const
{
    f32 x = (-mMatrix(0, 0) * mMatrix(0, 3) - mMatrix(1, 0) * mMatrix(1, 3)) -
            mMatrix(2, 0) * mMatrix(2, 3);
    f32 y = (-mMatrix(0, 1) * mMatrix(0, 3) - mMatrix(1, 1) * mMatrix(1, 3)) -
            mMatrix(2, 1) * mMatrix(2, 3);
    f32 z = (-mMatrix(0, 2) * mMatrix(0, 3) - mMatrix(1, 2) * mMatrix(1, 3)) -
            mMatrix(2, 2) * mMatrix(2, 3);
    dst->set(x, y, z);
}

void Camera::getLookVectorByMatrix(Vector3f* dst) const
{
    // Also known as the forward vector
    const Vector4f vec = mMatrix.getRow(2);
    dst->set(vec.x, vec.y, vec.z);
}

void Camera::getRightVectorByMatrix(Vector3f* dst) const
{
    const Vector4f vec = mMatrix.getRow(0);
    dst->set(vec.x, vec.y, vec.z);
}

void Camera::getUpVectorByMatrix(Vector3f* dst) const
{
    const Vector4f vec = mMatrix.getRow(1);
    dst->set(vec.x, vec.y, vec.z);
}

void Camera::worldPosToCameraPosByMatrix(Vector3f* dst, const Vector3f& world_pos) const
{
    dst->setMul(mMatrix, world_pos);
}

void Camera::cameraPosToWorldPosByMatrix(Vector3f* dst, const Vector3f& camera_pos) const
{
    Vector3f up;
    Vector3f right;
    Vector3f look;

    getUpVectorByMatrix(&up);
    getRightVectorByMatrix(&right);
    getLookVectorByMatrix(&look);

    up = up * camera_pos.y;
    look = look * camera_pos.z;
    right = right * camera_pos.x;

    Vector3f pos;
    getWorldPosByMatrix(&pos);
    pos = pos + up + look;

    dst->setAdd(pos, right);
}

void Camera::projectByMatrix(Vector2f* dst, const Vector3f& world_pos, const Projection& projection,
                             const Viewport& viewport) const
{
    Vector3f camera_pos;
    worldPosToCameraPosByMatrix(&camera_pos, world_pos);
    projection.project(dst, camera_pos, viewport);
}

void Camera::unprojectRayByMatrix(Ray<Vector3f>* dst, const Vector3f& camera_pos) const
{
    Vector3f up;
    Vector3f right;
    Vector3f look;

    getUpVectorByMatrix(&up);
    getRightVectorByMatrix(&right);
    getLookVectorByMatrix(&look);

    up = up * camera_pos.y;
    look = look * camera_pos.z;
    right = right * camera_pos.x;

    Vector3f dir;
    dir.setAdd(up, look);
    dir = dir + right;
    dir.normalize();

    Vector3f pos;
    getWorldPosByMatrix(&pos);
    dst->setPos(pos);
    dst->setDir(dir);
}

LookAtCamera::~LookAtCamera() = default;

LookAtCamera::LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
    : mPos(pos), mAt(at), mUp(up)
{
    SEAD_ASSERT(mPos != mAt);
    mUp.normalize();
}

void LookAtCamera::doUpdateMatrix(Matrix34f* dst) const
{
    if (mPos == mAt)
        return;

    Vector3f dir = mPos;
    dir -= mAt;
    dir.normalize();

    Vector3f right;
    right.setCross(mUp, dir);
    right.normalize();

    Vector3f up;
    up.setCross(dir, right);

    f32 x = -right.dot(mPos);
    f32 y = -up.dot(mPos);
    f32 z = -dir.dot(mPos);

    (*dst)(0, 0) = right.x;
    (*dst)(0, 1) = right.y;
    (*dst)(0, 2) = right.z;
    (*dst)(0, 3) = x;

    (*dst)(1, 0) = up.x;
    (*dst)(1, 1) = up.y;
    (*dst)(1, 2) = up.z;
    (*dst)(1, 3) = y;

    (*dst)(2, 0) = dir.x;
    (*dst)(2, 1) = dir.y;
    (*dst)(2, 2) = dir.z;
    (*dst)(2, 3) = z;
}

DirectCamera::~DirectCamera() = default;

void DirectCamera::doUpdateMatrix(Matrix34f* dst) const
{
    *dst = mDirectMatrix;
}

OrthoCamera::OrthoCamera() = default;

OrthoCamera::OrthoCamera(const OrthoProjection& projection)
    : LookAtCamera({0.5f * (projection.getLeft() + projection.getRight()),
                    0.5f * (projection.getTop() + projection.getBottom()),
                    projection.getNear()},
                   {0.5f * (projection.getLeft() + projection.getRight()),
                    0.5f * (projection.getTop() + projection.getBottom()),
                    projection.getNear() - 1.0f},
                   {0.0f, 1.0f, 0.0f})
{
}

OrthoCamera::~OrthoCamera() = default;

void OrthoCamera::setByOrthoProjection(const OrthoProjection& projection)
{
    setPos({0.5f * (projection.getLeft() + projection.getRight()),
            0.5f * (projection.getTop() + projection.getBottom()), projection.getNear()});
    setAt({0.5f * (projection.getLeft() + projection.getRight()),
           0.5f * (projection.getTop() + projection.getBottom()), projection.getNear() - 1.0f});
    setUp({0.0f, 1.0f, 0.0f});
}

}  // namespace sead
