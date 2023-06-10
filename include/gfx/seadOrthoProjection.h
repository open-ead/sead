#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class OrthoProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(OrthoProjection, Projection)
public:
    OrthoProjection();
    OrthoProjection(float near, float far, float top, float bottom, float left, float right);
    OrthoProjection(float, float, Viewport* viewport);
    ~OrthoProjection() override = default;

    float getNear() const override;
    float getFar() const override;
    float getFovy() const override;
    float getAspect() const override;
    void getOffset(sead::Vector2<float>* offsetVec) const override;
    void updateAttributesForDirectProjection() override;

    void setByViewport(sead::Viewport const& viewport);
    void setTBLR(float top, float bottom, float left, float right);

    void doUpdateMatrix(Matrix44f* mtx) const override;
    void doScreenPosToCameraPosTo(sead::Vector3<float>* pos_1,
                                  sead::Vector3<float> const& pos_2) const override;
    bool checkDerivedRuntimeTypeInfo(RuntimeTypeInfo::Interface* interface);
    RuntimeTypeInfo::Interface* getRuntimeTypeInfo();
    ProjectionType getProjectionType() const override;

private:
    float mNear;
    float mFar;
    float mTop;
    float mBottom;
    float mLeft;
    float mRight;
    // bool m01b;
};

}  // namespace sead
