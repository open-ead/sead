#pragma once

#include <gfx/seadProjection.h>

namespace sead
{
class PerspectiveProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(PerspectiveProjection, Projection)
public:
    PerspectiveProjection();
    PerspectiveProjection(float near, float far, float fovy, float aspect);
    ~PerspectiveProjection() override;

    float getNear() const override;
    float getFar() const override;
    float getFovy() const override;
    float getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;
    u32 getProjectionType() const override;

    void set(float, float, float, float);
    void doUpdateMatrix(Matrix44f* mtx);
    void setFovx(float);
    void createDividedProjection(sead::PerspectiveProjection* projection, int, int, int, int);
    float getTop();
    float getBottom();
    float getLeft();
    float getRight();
    void setTBLR(float top, float bottom, float left, float right);

private:
    float mNear;
    float mFar;
    float mFovy;
    Vector2f _a4;
    float _ac;
    float mAspect;
    Vector2f mOffset;
};
}  // namespace sead
