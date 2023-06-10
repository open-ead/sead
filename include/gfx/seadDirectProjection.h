#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class DirectProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(DirectProjection, Projection)
public:
    DirectProjection();
    DirectProjection(
        Matrix44f const& mtx,
        Graphics::DevicePosture posture);  // 0x7100b1e718 - Also initializes the Projection data
    ~DirectProjection() override = default;

    void setDirectProjectionMatrix(Matrix44f const& mtx, Graphics::DevicePosture posture);

    float getNear() const override;
    float getFar() const override;
    float getFovy() const override;
    float getAspect() const override;
    void getOffset(sead::Vector2<float>* offset) const override;
    void updateAttributesForDirectProjection() override;

    // void setByViewport(sead::Viewport const& viewport);
    // void setTBLR(float top, float bottom, float left, float right);

    // void doUpdateMatrix(Matrix44f* mtx) const override;
    // void doScreenPosToCameraPosTo(sead::Vector3<float>* pos_1,
    //                               sead::Vector3<float> const& pos_2) const override;
    ProjectionType getProjectionType() const override;

private:
    Matrix44f mDirectMatrix;
    float mNear;
    float mFar;
    float mFovy;
    float mAspect;
    Vector2f mOffset;
    float mUnknown7;
};

}  // namespace sead
