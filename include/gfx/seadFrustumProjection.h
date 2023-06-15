#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class FrustumProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(FrustumProjection, Projection)

public:
    FrustumProjection() = default;
    FrustumProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right);
    FrustumProjection(f32 _near, f32 _far, const BoundBox2f& box);
    ~FrustumProjection() override = default;

    Type getProjectionType() const override { return Projection::cType_Perspective; }
    void doUpdateMatrix(Matrix44f* dst) const override;
    void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const override;

    void setNear(f32 near);
    void setFar(f32 far);
    void setTop(f32 top);
    void setBottom(f32 bottom);
    void setLeft(f32 left);
    void setRight(f32 right);

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    void setBoundBox(const BoundBox2f& box);

    void createDividedProjection(FrustumProjection* dst, s32 partno_x, s32 partno_y, s32 divnum_x,
                                 s32 divnum_y) const;

    f32 getNear() const override { return mNear; }
    f32 getFar() const override { return mFar; }
    f32 getTop() const { return mTop; }
    f32 getBottom() const { return mBottom; }
    f32 getLeft() const { return mLeft; }
    f32 getRight() const { return mRight; }

    f32 getFovy() const override;
    f32 getAspect() const override;
    void getOffset(Vector2f* dst) const override;
    f32 getOffsetX() const;
    f32 getOffsetY() const;

    void setFovyAspectOffset(f32 fovy, f32 aspect, const Vector2f& offset);

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};

}  // namespace sead
