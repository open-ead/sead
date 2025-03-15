#ifndef SEAD_ORTHO_PROJECTION_H
#define SEAD_ORTHO_PROJECTION_H

#include <basis/seadTypes.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include <math/seadBoundBox.h>

namespace sead {

class OrthoProjection : public Projection {
    SEAD_RTTI_OVERRIDE(OrthoProjection, Projection);

public:
    OrthoProjection();
    OrthoProjection(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right);
    OrthoProjection(f32 near, f32 far, const BoundBox2f& boundBox);
    OrthoProjection(f32 near, f32 far, const Viewport& viewport);
    ~OrthoProjection() override;
    
    f32 getNear() const override;
    f32 getFar() const override;
    f32 getFovy() const override;
    f32 getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    u32 getProjectionType() const override;
    void doUpdateMatrix(Matrix44f* mtx) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;

    void createDividedProjection(OrthoProjection*, s32, s32, s32, s32) const;
    void setBoundBox(const BoundBox2f& boundBox);
    void setByViewport(const Viewport& viewport);
    void setTBLR(f32 top, f32 left, f32 bottom, f32 right);

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};

}

#endif
