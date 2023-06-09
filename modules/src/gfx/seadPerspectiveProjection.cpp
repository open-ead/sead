#include <cmath>
#include <gfx/seadPerspectiveProjection.h>

namespace sead
{

PerspectiveProjection::PerspectiveProjection(float near, float far, float fovy_rad, float aspect)
{
    mNear = near;
    mFar = far;
    mFovyRad = fovy_rad;
    mAspect = aspect;

    mFovySin = sinf((float)0.5 * fovy_rad);
    mFovyCos = cosf((float)0.5 * fovy_rad);
    mFovyTan = tanf((float)0.5 * fovy_rad);
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
    return mFovyCos;
}

float PerspectiveProjection::getAspect() const
{
    return mAspect;
}

Projection::ProjectionType PerspectiveProjection::getProjectionType() const
{
    return ProjectionType::cPerspectiveProjection;
}

// void PerspectiveProjection::setFovx_(float aFloat) {}

void PerspectiveProjection::setFovy_(float aFloat)
{
    mFovyCos = aFloat;
    mFovyTan = sinf((float)0.5 * aFloat);
    mOffset.set(cosf((float)0.5 * aFloat), tanf((float)0.5 * aFloat));
}

// void PerspectiveProjection::doUpdateMatrix(Matrix44f* mtx) const
// {
//     float temp = 1.0;
//     mtx->m[0][0] = (this->mFovyRad + this->mFovyRad) * (1.0 / temp);
//     mtx->m[0][1] = 0.0;
//     mtx->m[0][3] = 0.0;
// }

}  // namespace sead
