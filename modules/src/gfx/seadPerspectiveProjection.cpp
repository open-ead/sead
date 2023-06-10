#include <cmath>
#include <gfx/seadPerspectiveProjection.h>

namespace sead
{

PerspectiveProjection::PerspectiveProjection()
{
    mNear = 1.0;
    mFar = 1000.0;
    mOffset.set(float(4.0 / 3.0), 0.0);
    mFovyRad = M_PI / 4.0;  // pi / 4 radians or 45 degrees
    mFovySin = sinf(mFovyRad / (float)2.0);
    mFovyCos = cosf(mFovyRad / (float)2.0);
    mFovyTan = tanf(mFovyRad / (float)2.0);
    markDirty();
}

PerspectiveProjection::PerspectiveProjection(float near, float far, float fovy_rad, float aspect)
{
    mOffset.set((float)0.0, (float)0.0);
    set(near, far, fovy_rad, aspect);
}

float PerspectiveProjection::getNear() const
{
    return mNear;
}

float PerspectiveProjection::getFar() const
{
    return mFar;
}

float PerspectiveProjection::getFovy() const
{
    return mFovyRad;
}

float PerspectiveProjection::getAspect() const
{
    return mAspect;
}

void PerspectiveProjection::getOffset(Vector2f* offset) const
{
    offset->set(mOffset);
}

Projection::ProjectionType PerspectiveProjection::getProjectionType() const
{
    return ProjectionType::cPerspectiveProjection;
}

void PerspectiveProjection::set(float near, float far, float fovy_rad, float aspect)
{
    mNear = near;
    mFar = far;
    markDirty();
    mFovyRad = fovy_rad;

    mFovySin = sinf((float)0.5 * fovy_rad);
    mFovyCos = cosf((float)0.5 * fovy_rad);
    mFovyTan = tanf((float)0.5 * fovy_rad);

    mAspect = aspect;
    markDirty();
}

// set
// do Update

// void PerspectiveProjection::setFovx_(float aFloat) {}

void PerspectiveProjection::setFovy_(float newFovy)
{
    float local = (float)0.5 * newFovy;
    mFovyRad = newFovy;
    mFovySin = sinf(local);
    mFovyCos = cosf(local);
    mFovyTan = tanf(local);
    markDirty();
}

// void PerspectiveProjection::doUpdateMatrix(Matrix44f* mtx) const
// {
//     float temp = 1.0;
//     mtx->m[0][0] = (this->mFovyRad + this->mFovyRad) * (1.0 / temp);
//     mtx->m[0][1] = 0.0;
//     mtx->m[0][3] = 0.0;
// }

}  // namespace sead
