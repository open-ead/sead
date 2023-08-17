#pragma once
#include <gfx/seadProjection.h>

namespace sead
{

class OrthoProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(OrthoProjection, Projection)

public:
    OrthoProjection();
    OrthoProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right);
    OrthoProjection(f32 _near, f32 _far, const BoundBox2f& box);
    OrthoProjection(f32 _near, f32 _far, const Viewport& vp);
    ~OrthoProjection() override = default;

    void setNear(f32 near);
    void setFar(f32 far);
    void setTop(f32 top);
    void setBottom(f32 bottom);
    void setLeft(f32 left);
    void setRight(f32 right);
    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);
    void setByViewport(const Viewport& vp);
    void setBoundBox(const BoundBox2f& box);

    void createDividedProjection(OrthoProjection* dst, s32 partno_x, s32 partno_y, s32 divnum_x,
                                 s32 divnum_y) const;

    f32 getNear() const override { return mNear; }
    f32 getFar() const override { return mFar; }
    f32 getFovy() const override { return 0; }
    f32 getTop() const { return mTop; }
    f32 getBottom() const { return mBottom; }
    f32 getLeft() const { return mLeft; }
    f32 getRight() const { return mRight; }
    f32 getAspect() const override;
    void getOffset(Vector2f* offset) const override;

    Type getProjectionType() const override { return Projection::cType_Ortho; }
    void doUpdateMatrix(Matrix44f* dst) const override;
    void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const override;

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
    Vector2f mOffset;
};
#ifdef cafe
static_assert(sizeof(OrthoProjection) == 0xAC, "sead::OrthoProjection size mismatch");
#endif  // cafe

}  // namespace sead
