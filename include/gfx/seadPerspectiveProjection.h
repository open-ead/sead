#include <gfx/seadCamera.h>
#include <gfx/seadGraphics.h>
#include <gfx/seadProjection.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>
#include "math/seadMathNumbers.h"

namespace sead
{

class PerspectiveProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(PerspectiveProjection, Projection)

public:
    PerspectiveProjection();
    PerspectiveProjection(f32 near, f32 far, f32 fovy_rad, f32 aspect);
    ~PerspectiveProjection() override = default;

    void set(f32 _near, f32 _far, f32 fovy_rad, f32 aspect);

    void setNear(f32 near)
    {
        mNear = near;
        setDirty();
    }
    void setFar(f32 far)
    {
        mFar = far;
        setDirty();
    }
    void setFovx(f32 fovx);
    void setFovy(f32 fovy) { setFovy_(fovy); }
    void setAspect(f32 aspect)
    {
        mAspect = aspect;
        setDirty();
    }
    void setOffset(const Vector2f& offset)
    {
        mOffset = offset;
        setDirty();
    }

    void createDividedProjection(PerspectiveProjection* dst, s32 partno_x, s32 partno_y,
                                 s32 divnum_x, s32 divnum_y) const;

    f32 getNear() const override { return mNear; }
    f32 getFar() const override { return mFar; }
    f32 getFovy() const override { return mAngle; }
    f32 getAspect() const override { return mAspect; }
    void getOffset(Vector2f* offset) const override { offset->set(mOffset); }

    f32 getTop() const;
    f32 getBottom() const;
    f32 getLeft() const;
    f32 getRight() const;

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    Type getProjectionType() const override { return Projection::cType_Perspective; }
    void doUpdateMatrix(Matrix44f* dst) const override;
    void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const override;

private:
    void setFovy_(f32 fovy);

    f32 calcNearClipHeight_() const { return mNear * 2 * mFovyTan; }
    f32 calcNearClipWidth_() const { return calcNearClipHeight_() * mAspect; }

    f32 mNear = 1.0f;
    f32 mFar = 10000.0f;
    f32 mAngle;
    f32 mFovySin;
    f32 mFovyCos;
    f32 mFovyTan;
    f32 mAspect = 1.333333f;
    Vector2f mOffset = Vector2f::zero;
};
#ifdef cafe
static_assert(sizeof(PerspectiveProjection) == 0xB8, "sead::PerspectiveProjection size mismatch");
#endif  // cafe

}  // namespace sead
