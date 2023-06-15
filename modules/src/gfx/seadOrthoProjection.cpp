#include <gfx/seadOrthoProjection.h>
#include <gfx/seadViewport.h>
namespace sead
{
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

void OrthoProjection::setByViewport(const Viewport& vp)
{
    Vector2f max = vp.getMax();
    Vector2f min = vp.getMin();
    mTop = (float)0.5 * (max.y - min.y);
    mBottom = -(float)0.5 * (max.y - min.y);
    mLeft = -(float)0.5 * (max.x - min.x);
    mRight = (float)0.5 * (max.x - min.x);
    setDirty();
}

}  // namespace sead
