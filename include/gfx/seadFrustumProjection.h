#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class FrustumProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(FrustumProjection, Projection)
public:
    FrustumProjection();
    FrustumProjection(float near, float far, float top, float bottom, float left, float right);
    ~FrustumProjection() override = default;

    float getNear() const override;
    float getFar() const override;
    float getFovy() const override;
    float getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    void updateAttributesForDirectProjection() override;
    ProjectionType getProjectionType() const override;
    void doUpdateMatrix(Matrix44f* mtx) const override;
    void doScreenPosToCameraPosTo(Vector3f*, const Vector3f&) const override;

private:
    float mNear;
    float mFar;
    float mTop;
    float mBottom;
    float mLeft;
    float mRight;
};

}  // namespace sead
