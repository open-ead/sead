#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class PerspectiveProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(PerspectiveProjection, Projection)
public:
    PerspectiveProjection();
    PerspectiveProjection(float near, float far, float fovy_rad, float aspect);
    ~PerspectiveProjection() override;

    float getNear() const override;
    float getFar() const override;
    float getFovy() const override;
    float getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;
    ProjectionType getProjectionType() const override;

    void set(float near, float far, float fovy_rad, float aspect);
    void doUpdateMatrix(Matrix44f* mtx) const override;
    void setFovx_(float aFloat);
    void setFovy_(float aFloat);
    void createDividedProjection(sead::PerspectiveProjection* projection, int, int, int, int);
    float getTop();
    float getBottom();
    float getLeft();
    float getRight();
    void setTBLR(float top, float bottom, float left, float right);

private:
    float mNear;
    float mFar;
    float mFovyRad;
    float mFovySin;
    float mFovyCos;
    float mFovyTan;
    float mAspect;
    Vector2f mOffset;
};

}  // namespace sead
