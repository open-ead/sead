#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>

namespace sead
{
Projection::Projection()
    : mDirty(true), mDeviceDirty(true), mDevicePosture(Graphics::getDefaultDevicePosture()),
      mDeviceZScale(Graphics::getDefaultDeviceZScale()),
      mDeviceZOffset(Graphics::getDefaultDeviceZOffset())
{
}

void Projection::updateMatrixImpl_() const
{
    if (mDirty)
    {
        doUpdateMatrix(const_cast<Matrix44f*>(&mMatrix));
        mDirty = false;
        mDeviceDirty = true;
    }

    if (mDeviceDirty)
    {
        doUpdateDeviceMatrix(const_cast<Matrix44f*>(&mDeviceMatrix), mMatrix, mDevicePosture);
        mDeviceDirty = false;
    }
}

const Matrix44f& Projection::getProjectionMatrix() const
{
    updateMatrixImpl_();
    return mMatrix;
}

const Matrix44f& Projection::getDeviceProjectionMatrix() const
{
    updateMatrixImpl_();
    return mDeviceMatrix;
}

void Projection::project(Vector2f* dst, const Vector3f& camera_pos, const Viewport& viewport) const
{
    Vector3f temp;
    doScreenPosToCameraPosTo(&temp, camera_pos);
    viewport.project(dst, temp);
}

void Projection::unproject(Vector3f* dst, const Vector3f& screen_pos, const Camera& camera) const
{
    doScreenPosToCameraPosTo(dst, screen_pos);
    camera.cameraPosToWorldPosByMatrix(dst, screen_pos);
}

void Projection::unprojectRay(Ray<Vector3f>* dst, const Vector3f& screen_pos,
                              const Camera& camera) const
{
    // doScreenPosToCameraPosTo(dst, screen_pos);
    // camera.cameraPosToWorldPosByMatrix(dst, screen_pos);
}

void Projection::updateAttributesForDirectProjection() {}

void Projection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    updateMatrixImpl_();

    f32 scale = 1.0f / (mMatrix(3, 3) + screen_pos.x * mMatrix(3, 0) +
                        screen_pos.y * mMatrix(3, 1) + screen_pos.z * mMatrix(3, 2));
    dst->x = scale * (mMatrix(3, 3) + screen_pos.x * mMatrix(0, 0) + screen_pos.y * mMatrix(1, 0) +
                      screen_pos.z * mMatrix(2, 0));
    dst->y = scale * (mMatrix(1, 3) + screen_pos.x * mMatrix(1, 0) + screen_pos.y * mMatrix(1, 1) +
                      screen_pos.z * mMatrix(1, 2));
    dst->z = scale * (mMatrix(2, 3) + screen_pos.x * mMatrix(2, 0) + screen_pos.y * mMatrix(2, 1) +
                      screen_pos.z * mMatrix(2, 2));
}

namespace
{

void swapMatrixXY(Matrix44f* tgt)
{
    Vector4f x;
    Vector4f y;
    tgt->getRow(x, 0);
    tgt->getRow(y, 1);
    tgt->setRow(0, y);
    tgt->setRow(1, x);
}

}  // namespace

void Projection::doUpdateDeviceMatrix(Matrix44f* dst, const Matrix44f& src,
                                      Graphics::DevicePosture pose) const
{
    *dst = src;

    switch (pose)
    {
    case Graphics::cDevicePosture_Same:
        break;
    case Graphics::cDevicePosture_RotateRight:
        (*dst)(0, 0) *= -1;
        (*dst)(0, 1) *= -1;
        (*dst)(0, 2) *= -1;
        (*dst)(0, 3) *= -1;
        swapMatrixXY(dst);
        break;
    case Graphics::cDevicePosture_RotateLeft:
        (*dst)(1, 0) *= -1;
        (*dst)(1, 1) *= -1;
        (*dst)(1, 2) *= -1;
        (*dst)(1, 3) *= -1;
        swapMatrixXY(dst);
        break;
    case Graphics::cDevicePosture_RotateHalfAround:
        (*dst)(0, 0) *= -1;
        (*dst)(0, 1) *= -1;
        (*dst)(0, 2) *= -1;
        (*dst)(0, 3) *= -1;
        (*dst)(1, 0) *= -1;
        (*dst)(1, 1) *= -1;
        (*dst)(1, 2) *= -1;
        (*dst)(1, 3) *= -1;
        break;
    case Graphics::cDevicePosture_FlipX:
        (*dst)(0, 0) *= -1;
        (*dst)(0, 1) *= -1;
        (*dst)(0, 2) *= -1;
        (*dst)(0, 3) *= -1;
        break;
    case Graphics::cDevicePosture_FlipY:
        (*dst)(1, 0) *= -1;
        (*dst)(1, 1) *= -1;
        (*dst)(1, 2) *= -1;
        (*dst)(1, 3) *= -1;
        break;
    default:;
        // SEAD_ASSERTMSG(false, "Invalid DevicePosture(%d).", s32(pose));
    }

    (*dst)(2, 0) = (*dst)(2, 0) * mDeviceZScale;
    (*dst)(2, 1) = (*dst)(2, 1) * mDeviceZScale;

    (*dst)(2, 2) = ((*dst)(2, 2) + (*dst)(3, 2) * mDeviceZOffset) * mDeviceZScale;
    (*dst)(2, 3) = (*dst)(2, 3) * mDeviceZScale + (*dst)(3, 3) * mDeviceZOffset;
}

}  // namespace sead
