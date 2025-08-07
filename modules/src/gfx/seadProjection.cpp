#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include "math/seadMatrixCalcCommon.h"
#include "math/seadVectorCalcCommon.h"

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

void Projection::unproject(Vector3f* dst, const Vector3f& screenPos, const Camera& camera) const
{
    doScreenPosToCameraPosTo(dst, screenPos);
    camera.cameraPosToWorldPosByMatrix(dst, screenPos);
}

void Projection::unprojectRay(Ray<Vector3f>* dst, const Vector3f& screenPos,
                              const Camera& camera) const
{
    Vector3f newVec;
    doScreenPosToCameraPosTo(dst, screenPos);
    camera.unprojectRayByMatrix(dst, newVec);
}

void Projection::updateAttributesForDirectProjection() {}

void Projection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    updateMatrixImpl_();
    f32 scale = 1.0f / (mMatrix(3, 3) + screen_pos.x * mMatrix(3, 0) +
                        screen_pos.y * mMatrix(3, 1) + screen_pos.z * mMatrix(3, 2));
    *dst = static_cast<Matrix34f>(mMatrix) * screen_pos;
    *dst *= scale;
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

PerspectiveProjection::PerspectiveProjection()
{
    setFovy_(mAngle);  // 45 degrees
}

PerspectiveProjection::PerspectiveProjection(f32 near, f32 far, f32 fovy_rad, f32 aspect)
    : mNear(near), mFar(far), mAspect(aspect), mOffset(Vector2f::zero)
{
    setFovy_(fovy_rad);
}

void PerspectiveProjection::set(f32 _near, f32 _far, f32 fovy_rad, f32 aspect)
{
    setNear(_near);
    setFar(_far);
    setFovy_(fovy_rad);
    setAspect(aspect);
}

void PerspectiveProjection::setFovy_(f32 fovy)
{
    mAngle = fovy;

    fovy *= 0.5f;
    mFovySin = Mathf::sin(fovy);
    mFovyCos = Mathf::cos(fovy);
    mFovyTan = Mathf::tan(fovy);

    setDirty();
}

f32 PerspectiveProjection::getTop() const
{
    f32 clip_height = calcNearClipHeight_();
    f32 center_y = mOffset.y * clip_height;
    return clip_height * 0.5f + center_y;
}

f32 PerspectiveProjection::getBottom() const
{
    f32 clip_height = calcNearClipHeight_();
    f32 center_y = mOffset.y * clip_height;
    return -clip_height * 0.5f + center_y;
}

f32 PerspectiveProjection::getLeft() const
{
    f32 clip_width = calcNearClipWidth_();
    f32 center_x = mOffset.x * clip_width;
    return -clip_width * 0.5f + center_x;
}

f32 PerspectiveProjection::getRight() const
{
    f32 clip_width = calcNearClipWidth_();
    f32 center_x = mOffset.x * clip_width;
    return clip_width * 0.5f + center_x;
}

void PerspectiveProjection::doUpdateMatrix(Matrix44f* dst) const
{
    f32 clip_height = calcNearClipHeight_();
    f32 clip_width = calcNearClipWidth_();

    f32 center_x = clip_width * mOffset.x;
    f32 center_y = clip_height * mOffset.y;

    clip_height *= 0.5f;
    clip_width *= 0.5f;

    f32 top = clip_height + center_y;
    f32 bottom = -clip_height + center_y;

    f32 left = -clip_width + center_x;
    f32 right = clip_width + center_x;

    f32 inv_size = 1.0f / (right - left);

    (*dst)(0, 0) = mNear * 2 * inv_size;
    (*dst)(0, 1) = 0.0f;
    (*dst)(0, 2) = (right + left) * inv_size;
    (*dst)(0, 3) = 0.0f;

    inv_size = 1.0f / (top - bottom);

    (*dst)(1, 0) = 0.0f;
    (*dst)(1, 1) = mNear * 2 * inv_size;
    (*dst)(1, 2) = (top + bottom) * inv_size;
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

void PerspectiveProjection::doScreenPosToCameraPosTo(Vector3f* dst,
                                                     const Vector3f& screen_pos) const
{
    dst->set(0.0f, 0.0f, -mNear);

    dst->y = (calcNearClipHeight_() / 2) * (screen_pos.y + mOffset.y * 2);
    dst->x = (calcNearClipWidth_() / 2) * (screen_pos.x + mOffset.x * 2);
}

OrthoProjection::OrthoProjection()
    : mNear(0.0), mFar(1.0), mTop(0.5), mBottom(-0.5), mLeft(-0.5), mRight(0.5)
{
}

OrthoProjection::OrthoProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right)
    : mNear(_near), mFar(_far), mTop(top), mBottom(bottom), mLeft(left), mRight(right)
{
    setDirty();
}

OrthoProjection::OrthoProjection(f32 _near, f32 _far, const Viewport& vp) : mNear(_near), mFar(_far)
{
    setByViewport(vp);
    setDirty();
}

f32 OrthoProjection::getAspect() const
{
    return (mRight - mLeft) / (mTop - mBottom);
}

void OrthoProjection::getOffset(Vector2f* offset) const
{
    offset->x = ((float)0.5 * (this->mLeft + this->mRight)) / (this->mRight - this->mLeft);
    offset->y = ((float)0.5 * (this->mTop + this->mBottom)) / (this->mTop - this->mBottom);
}

void OrthoProjection::setNear(f32 near)
{
    mNear = near;
    setDirty();
}

void OrthoProjection::setFar(f32 far)
{
    mFar = far;
    setDirty();
}

void OrthoProjection::setTop(f32 top)
{
    mTop = top;
    setDirty();
}

void OrthoProjection::setBottom(f32 bottom)
{
    mBottom = bottom;
    setDirty();
}

void OrthoProjection::setLeft(f32 left)
{
    mLeft = left;
    setDirty();
}

void OrthoProjection::setRight(f32 right)
{
    mRight = right;
    setDirty();
}

void OrthoProjection::setTBLR(float top, float bottom, float left, float right)
{
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    setDirty();
    // this->projectionType = ProjectionType::cOrthoProjection;
}

void OrthoProjection::doUpdateMatrix(Matrix44f* dst) const
{
    f32 inv_size = (mRight - mLeft) * 0.5f;

    (*dst)(0, 0) = 1.0f / inv_size;
    (*dst)(0, 1) = 0.0f;
    (*dst)(0, 2) = 0.0f;
    (*dst)(0, 3) = -0.5f * (mLeft + mRight) / inv_size;

    inv_size = 0.5f * (mTop - mBottom);

    (*dst)(1, 0) = 0;
    (*dst)(1, 1) = 1.0f / inv_size;
    (*dst)(1, 2) = 0;
    (*dst)(1, 3) = -0.5f * (mTop + mBottom) / inv_size;

    inv_size = 0.5f * (mFar - mNear);

    (*dst)(2, 0) = 0;
    (*dst)(2, 1) = 0;
    (*dst)(2, 2) = -1.0f / inv_size;
    (*dst)(2, 3) = -0.5f * (mNear * mFar) / inv_size;

    (*dst)(3, 0) = 0;
    (*dst)(3, 1) = 0;
    (*dst)(3, 2) = 0;
    (*dst)(3, 3) = -1.0f;
}

// NON-MATCHING: Adds an additional register for a multiplication
void OrthoProjection::setByViewport(const Viewport& vp)
{
    f32 halfY = vp.getHalfSizeY();
    f32 halfX = vp.getHalfSizeX();
    mTop = halfY;
    mBottom = -halfY;
    mLeft = -halfX;
    mRight = halfX;
    setDirty();
}

void OrthoProjection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    dst->x = 0.5f * ((mRight + mLeft) + screen_pos.x * (mRight - mLeft));
    dst->y = 0.5f * ((mTop + mBottom) + (screen_pos.y * (mTop - mBottom)));
    dst->z = mNear;
}

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
