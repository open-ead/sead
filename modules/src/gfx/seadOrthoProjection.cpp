#include <gfx/seadOrthoProjection.h>
namespace sead
{

OrthoProjection::OrthoProjection(float near, float far, float top, float bottom, float left,
                                 float right)
{
    mNear = near;
    mFar = far;
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    markDirty();
}

Projection::ProjectionType OrthoProjection::getProjectionType() const
{
    return ProjectionType::cOrthoProjection;
}

float OrthoProjection::getNear() const
{
    return this->mNear;
}

float OrthoProjection::getFar() const
{
    return this->mFar;
}

float OrthoProjection::getFovy() const
{
    return 0;
}

float OrthoProjection::getAspect() const
{
    return (this->mRight - this->mLeft) / (this->mTop - this->mBottom);
}

void OrthoProjection::getOffset(sead::Vector2f* offsetVec) const
{
    offsetVec->x = ((float)0.5 * (this->mLeft + this->mRight)) / (this->mRight - this->mLeft);
    offsetVec->y = ((float)0.5 * (this->mTop + this->mBottom)) / (this->mTop - this->mBottom);
}

void OrthoProjection::setTBLR(float top, float bottom, float left, float right)
{
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;
    markDirty();
    // this->projectionType = ProjectionType::cOrthoProjection;
}

// Decompiled code maybe makes assumption about initial state of the incoming matrix?
void OrthoProjection::doUpdateMatrix(Matrix44f* mtx) const
{
    // See https://learnwebgl.brown37.net/08_projections/projections_perspective.html for a
    // description of the construction of the orthographic projection matrix.

    // There seems to be an assumption about the near (1) and far (2) projection planes

    mtx->m[0][0] = (float)1.0 / ((mRight - mLeft) * (float)0.5);
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[0][3] = -(float)1.0 * (mLeft + mRight) / (mRight - mLeft);

    mtx->m[1][0] = 0;
    mtx->m[1][1] = (float)1.0 / ((mTop - mBottom) * (float)0.5);
    mtx->m[1][2] = 0;
    mtx->m[1][3] = -(float)1.0 * (mTop + mBottom) / (mTop - mBottom);

    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = -(float)2.0 / (mFar - mNear);
    mtx->m[2][3] = -((float)2.0 * mNear * mFar) / (mFar - mNear);

    mtx->m[3][0] = 0;
    mtx->m[3][1] = 0;
    mtx->m[3][2] = -(float)1.0;
    mtx->m[3][3] = 0;
}

void OrthoProjection::setByViewport(sead::Viewport const& viewport)
{
    Vector2f max = viewport.getMax();
    Vector2f min = viewport.getMin();
    mTop = (float)0.5 * (max.y - min.y);
    mBottom = -(float)0.5 * (max.y - min.y);
    mLeft = -(float)0.5 * (max.x - min.x);
    mRight = (float)0.5 * (max.x - min.x);
    markDirty();
}

}  // namespace sead
