#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class DirectProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(DirectProjection, Projection)

public:
    DirectProjection();
    DirectProjection(const Matrix44f* mtx, Graphics::DevicePosture posture);
    ~DirectProjection() override = default;

    void updateAttributesForDirectProjection() override;
    Type getProjectionType() const override { return cType_Undefined; }
    void doUpdateMatrix(Matrix44f* dst) const override;

    void setDirectProjectionMatrix(const Matrix44f* mtx, Graphics::DevicePosture posture);

    f32 getNear() const override { return mNear; }
    f32 getFar() const override { return mFar; }
    f32 getFovy() const override { return mFovy; }

    f32 getAspect() const override { return mAspect; }
    void getOffset(Vector2f* offset) const override { *offset = mOffset; }

    void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const override;

private:
    Matrix44f mDirectMatrix = Matrix44f::ident;
    f32 mNear = 0.0;
    f32 mFar = 0.0;
    f32 mFovy = 0.0;
    f32 mAspect = 0.0;
    Vector2f mOffset = Vector2f::zero;
    bool someBool = true;
};
#ifdef cafe
static_assert(sizeof(FrustumProjection) == 0xAC, "sead::FrustumProjection size mismatch");
#endif  // cafe

}  // namespace sead
