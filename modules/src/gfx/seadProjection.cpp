#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include "math/seadMatrixCalcCommon.h"
#include "math/seadVectorCalcCommon.h"

namespace sead
{
Projection::Projection()
{
    mDevicePosture = Graphics::sDefaultDevicePosture;
    mDeviceZScale = Graphics::sDefaultDeviceZScale;
    mDeviceZOffset = Graphics::sDefaultDeviceZOffset;
}

void Projection::updateAttributesForDirectProjection() {}

const Matrix44f& Projection::getProjectionMatrix() const
{
    updateMatrixImpl_();
    return mMatrix;
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

Matrix44f* Projection::getProjectionMatrixMutable()
{
    updateMatrixImpl_();
    return &mMatrix;
}

const Matrix44f& Projection::getDeviceProjectionMatrix() const
{
    updateMatrixImpl_();
    return mDeviceMatrix;
}

void Projection::cameraPosToScreenPos(Vector3f* screen_pos, const Vector3f& camera_pos) const
{
    screen_pos->setMul(getProjectionMatrix(), camera_pos);
}

void Projection::screenPosToCameraPos(Vector3f* camera_pos, const Vector3f& screen_pos) const
{
    doScreenPosToCameraPosTo(camera_pos, screen_pos);
}

void Projection::screenPosToCameraPos(Vector3f* camera_pos, const Vector2f& screen_pos) const
{
    screenPosToCameraPos(camera_pos, {screen_pos.x, screen_pos.y, 0.0f});
}

void Projection::project(Vector2f* dst, const Vector3f& camera_pos, const Viewport& viewport) const
{
    Vector3f screen_pos;
    cameraPosToScreenPos(&screen_pos, camera_pos);
    viewport.project(dst, screen_pos);
}

void Projection::unproject(Vector3f* world_pos, const Vector3f& screen_pos,
                           const Camera& camera) const
{
    Vector3f camera_pos;
    screenPosToCameraPos(&camera_pos, screen_pos);
    camera.cameraPosToWorldPosByMatrix(world_pos, camera_pos);
}

void Projection::unprojectRay(Ray<Vector3f>* dst, const Vector3f& screen_pos,
                              const Camera& camera) const
{
    Vector3f camera_pos;
    screenPosToCameraPos(&camera_pos, screen_pos);
    camera.unprojectRayByMatrix(dst, camera_pos);
}

static void swapMatrixXY(Matrix44f* mtx)
{
    Vector4f x = mtx->getRow(0);
    mtx->setRow(0, mtx->getRow(1));
    mtx->setRow(1, x);
}

static void negateRow(Matrix44f* mtx, s32 row)
{
    (*mtx)(row, 0) *= -1;
    (*mtx)(row, 1) *= -1;
    (*mtx)(row, 2) *= -1;
    (*mtx)(row, 3) *= -1;
}

static void negateMatrixRow(Matrix44f* mtx, s32 row)
{
    Vector4f* p = reinterpret_cast<Vector4f*>(mtx) + row;
    Vector4f v = *p;
    v.negate();
    *p = v;
}

void Projection::doUpdateDeviceMatrix(Matrix44f* dst, const Matrix44f& src,
                                      Graphics::DevicePosture pose) const
{
    *dst = src;

    switch (pose)
    {
    case Graphics::cDevicePosture_Same:
        break;
    case Graphics::cDevicePosture_RotateLeft:
        negateRow(dst, 1);
        swapMatrixXY(dst);
        break;
    case Graphics::cDevicePosture_RotateRight:
        negateRow(dst, 0);
        swapMatrixXY(dst);
        break;
    case Graphics::cDevicePosture_RotateHalfAround:
        negateRow(dst, 0);
        negateRow(dst, 1);
        break;
    case Graphics::cDevicePosture_FlipX:
        negateRow(dst, 0);
        break;
    case Graphics::cDevicePosture_FlipY:
        for (s32 i = 0; i < 4; ++i)
            (*dst)(1, i) *= -1;
        break;
    default:
        break;
    }

    (*dst)(2, 0) = (*dst)(2, 0) * mDeviceZScale;
    (*dst)(2, 1) = (*dst)(2, 1) * mDeviceZScale;
    (*dst)(2, 2) = ((*dst)(2, 2) + (*dst)(3, 2) * mDeviceZOffset) * mDeviceZScale;
    (*dst)(2, 3) = (*dst)(2, 3) * mDeviceZScale + (*dst)(3, 3) * mDeviceZOffset;
}

PerspectiveProjection::PerspectiveProjection()
{
    setFovy_(numbers::pi / 4.0f);  // 45 degrees
}

PerspectiveProjection::PerspectiveProjection(f32 near, f32 far, f32 fovy_rad, f32 aspect)
    : mNear(near), mFar(far), mAspect(aspect), mOffset(Vector2f::zero)
{
    setFovy_(fovy_rad);
}

PerspectiveProjection::~PerspectiveProjection() = default;

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

f32 PerspectiveProjection::getNear() const
{
    return mNear;
}

f32 PerspectiveProjection::getFar() const
{
    return mFar;
}

f32 PerspectiveProjection::getFovy() const
{
    return mAngle;
}

f32 PerspectiveProjection::getAspect() const
{
    return mAspect;
}

void PerspectiveProjection::getOffset(Vector2f* offset) const
{
    offset->x = mOffset.x;
    offset->y = mOffset.y;
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
    setDirty();
}

OrthoProjection::OrthoProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right)
    : mNear(_near), mFar(_far), mTop(top), mBottom(bottom), mLeft(left), mRight(right)
{
    setDirty();
}

OrthoProjection::OrthoProjection(f32 _near, f32 _far, const Viewport& vp) : mNear(_near), mFar(_far)
{
    mTop = 0.5f * vp.getSizeY();
    mBottom = -0.5f * vp.getSizeY();
    mLeft = -0.5f * vp.getSizeX();
    mRight = 0.5f * vp.getSizeX();
    setDevicePosture(vp.getDevicePosture());
    setDirty();
}

f32 OrthoProjection::getNear() const
{
    return mNear;
}

f32 OrthoProjection::getFar() const
{
    return mFar;
}

f32 OrthoProjection::getFovy() const
{
    return 0;
}

f32 OrthoProjection::getAspect() const
{
    return (mRight - mLeft) / (mTop - mBottom);
}

void OrthoProjection::getOffset(Vector2f* offset) const
{
    offset->x = (0.5f * (mLeft + mRight)) / (mRight - mLeft);
    offset->y = (0.5f * (mTop + mBottom)) / (mTop - mBottom);
}

void OrthoProjection::setTBLR(float top, float bottom, float left, float right)
{
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    setDirty();
}

void OrthoProjection::doUpdateMatrix(Matrix44f* dst) const
{
    f32 sum_x = mLeft + mRight;
    f32 sum_y = mTop + mBottom;
    f32 inv_size = (mRight - mLeft) * 0.5f;

    (*dst)(0, 0) = 1.0f / inv_size;
    (*dst)(0, 1) = 0.0f;
    (*dst)(0, 2) = 0.0f;
    (*dst)(0, 3) = sum_x * -0.5f / inv_size;

    inv_size = (mTop - mBottom) * 0.5f;

    (*dst)(1, 0) = 0;
    (*dst)(1, 1) = 1.0f / inv_size;
    (*dst)(1, 2) = 0;
    (*dst)(1, 3) = sum_y * -0.5f / inv_size;

    f32 inv_depth = 1.0f / (mFar - mNear);

    (*dst)(2, 0) = 0;
    (*dst)(2, 1) = 0;
    (*dst)(2, 2) = inv_depth * -2.0f;
    (*dst)(2, 3) = -(inv_depth * (mNear + mFar));

    (*dst)(3, 0) = 0;
    (*dst)(3, 1) = 0;
    (*dst)(3, 2) = 0;
    (*dst)(3, 3) = 1.0f;
}

void OrthoProjection::setByViewport(const Viewport& vp)
{
    f32 top = 0.5f * vp.getSizeY();
    f32 bottom = -0.5f * vp.getSizeY();
    f32 right = 0.5f * vp.getSizeX();
    f32 left = -0.5f * vp.getSizeX();
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    setDirty();
}

void OrthoProjection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    dst->x = screen_pos.x * (mRight - mLeft) * 0.5f + (mRight + mLeft) * 0.5f;
    dst->y = screen_pos.y * (mTop - mBottom) * 0.5f + (mTop + mBottom) * 0.5f;
    dst->z = -mNear;
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

f32 FrustumProjection::getNear() const
{
    return mNear;
}

f32 FrustumProjection::getFar() const
{
    return mFar;
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

void FrustumProjection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    dst->z = -mNear;
    dst->x = (mRight - mLeft) * screen_pos.x * 0.5f + (mRight + mLeft) * 0.5f;
    dst->y = (mTop - mBottom) * screen_pos.y * 0.5f + (mTop + mBottom) * 0.5f;
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
    f32 denom = mRight - mLeft;
    dst->x = 0.5f * (mRight + mLeft) / denom;

    denom = mTop - mBottom;
    dst->y = 0.5f * (mTop + mBottom) / denom;
}

f32 FrustumProjection::getOffsetX() const
{
    f32 den = mRight - mLeft;
    f32 num = mRight + mLeft;
    return 0.5f * num / den;
}

f32 FrustumProjection::getOffsetY() const
{
    f32 den = mTop - mBottom;
    f32 num = mTop + mBottom;
    return 0.5f * num / den;
}

DirectProjection::DirectProjection() : mDirectMatrix(Matrix44f::ident)
{
    setDirty();
}

DirectProjection::DirectProjection(const Matrix44f& mtx, Graphics::DevicePosture posture)
{
    setProjectionMatrix(mtx, posture);
}

static bool isXShifted(const Vector3f& near_corner, const Vector3f& far_corner)
{
    const f32 d = near_corner.x - far_corner.x;
    return (d > 0.0f ? d : -d) > 0.0001f;
}

void DirectProjection::updateAttributesForDirectProjection()
{
    if (!mAttributesDirty)
    {
        return;
    }

    Matrix44f inv;
    inv.setInverse(mDirectMatrix);

    // Corners 0-3 are near plane quad, 4-7 are far-plane quad (Z sign indicates)
    const Vector4f corners[8] = {
        {-1, -1, -1, 1}, {-1, 1, -1, 1}, {1, 1, -1, 1}, {1, -1, -1, 1},
        {-1, -1, 1, 1},  {-1, 1, 1, 1},  {1, 1, 1, 1},  {1, -1, 1, 1},
    };

    Vector3f cameraSpaceCorners[8];
    for (s32 i = 0; i < 8; ++i)
    {
        const Vector4f& c = corners[i];
        Vector3f p;
        p.x = c.x * inv(0, 0) + c.y * inv(0, 1) + c.z * inv(0, 2) + c.w * inv(0, 3);
        p.y = c.x * inv(1, 0) + c.y * inv(1, 1) + c.z * inv(1, 2) + c.w * inv(1, 3);
        p.z = c.x * inv(2, 0) + c.y * inv(2, 1) + c.z * inv(2, 2) + c.w * inv(2, 3);
        const f32 inv_w =
            1.0f / (c.x * inv(3, 0) + c.y * inv(3, 1) + c.z * inv(3, 2) + c.w * inv(3, 3));
        cameraSpaceCorners[i] = p * inv_w;
    }

    mNear = -cameraSpaceCorners[0].z;
    mFar = -cameraSpaceCorners[4].z;
    f32 height = cameraSpaceCorners[1].y - cameraSpaceCorners[0].y;
    mAspect = (cameraSpaceCorners[2].x - cameraSpaceCorners[0].x) / height;
    mOffset.x = (cameraSpaceCorners[0].x + cameraSpaceCorners[2].x) * 0.5f /
                (cameraSpaceCorners[2].x - cameraSpaceCorners[0].x);
    mOffset.y = (cameraSpaceCorners[1].y + cameraSpaceCorners[0].y) * 0.5f / height;

    if (isXShifted(cameraSpaceCorners[0], cameraSpaceCorners[4]) ||
        isXShifted(cameraSpaceCorners[1], cameraSpaceCorners[5]) ||
        isXShifted(cameraSpaceCorners[2], cameraSpaceCorners[6]) ||
        isXShifted(cameraSpaceCorners[3], cameraSpaceCorners[7]))
    {
        mFovy = 2 * Mathf::atan2(height * 0.5f, mNear);
    }
    else
    {
        mFovy = 0.0f;
    }

    mAttributesDirty = false;
}

void DirectProjection::doUpdateMatrix(Matrix44f* dst) const
{
    *dst = mDirectMatrix;
}

void DirectProjection::setProjectionMatrix(const Matrix44f& mtx, Graphics::DevicePosture posture)
{
    mDirectMatrix = mtx;

    s32 negate_row = -1;

    switch (posture)
    {
    case Graphics::cDevicePosture_Same:
        break;
    case Graphics::cDevicePosture_RotateLeft:
        negate_row = 1;
        swapMatrixXY(&mDirectMatrix);
        break;
    case Graphics::cDevicePosture_RotateRight:
        negate_row = 0;
        swapMatrixXY(&mDirectMatrix);
        break;
    case Graphics::cDevicePosture_RotateHalfAround:
        negateMatrixRow(&mDirectMatrix, 0);
        negate_row = 1;
        break;
    case Graphics::cDevicePosture_FlipX:
        negate_row = 0;
        break;
    case Graphics::cDevicePosture_FlipY:
        negate_row = 1;
        break;
    default:
        break;
    }

    if (negate_row >= 0)
    {
        negateMatrixRow(&mDirectMatrix, negate_row);
    }

    setDirty();
    mAttributesDirty = true;
}

f32 DirectProjection::getNear() const
{
    return mNear;
}

f32 DirectProjection::getFar() const
{
    return mFar;
}

f32 DirectProjection::getFovy() const
{
    return mFovy;
}

f32 DirectProjection::getAspect() const
{
    return mAspect;
}

void DirectProjection::getOffset(Vector2f* offset) const
{
    *offset = mOffset;
}

void DirectProjection::doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const
{
    Matrix44f inverseDirect;
    inverseDirect.setInverse(mDirectMatrix);
    const f32 x = screen_pos.x;
    const f32 y = screen_pos.y;
    const f32 z = screen_pos.z;
    f32 scale = 1.0f / (x * inverseDirect(3, 0) + y * inverseDirect(3, 1) +
                        z * inverseDirect(3, 2) + inverseDirect(3, 3));
    dst->x = scale * (x * inverseDirect(0, 0) + y * inverseDirect(0, 1) + z * inverseDirect(0, 2) +
                      inverseDirect(0, 3));
    dst->y = scale * (x * inverseDirect(1, 0) + y * inverseDirect(1, 1) + z * inverseDirect(1, 2) +
                      inverseDirect(1, 3));
    dst->z = scale * (x * inverseDirect(2, 0) + y * inverseDirect(2, 1) + z * inverseDirect(2, 2) +
                      inverseDirect(2, 3));
}

}  // namespace sead
