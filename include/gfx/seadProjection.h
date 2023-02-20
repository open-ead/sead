#ifndef SEAD_PROJECTION_H_
#define SEAD_PROJECTION_H_

#include <basis/seadTypes.h>
#include <gfx/seadGraphics.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead
{
class Projection
{
    SEAD_RTTI_BASE(Projection)

public:
    Projection();
    virtual ~Projection();

    virtual float getNear() const = 0;
    virtual float getFar() const = 0;
    virtual float getFovy() const = 0;
    virtual float getAspect() const = 0;
    virtual void getOffset(Vector2f* offset) const = 0;
    virtual void updateAttributesForDirectProjection();
    virtual u32 getProjectionType() const = 0;
    virtual void doUpdateMatrix(Matrix44f* mtx) const = 0;
    virtual void doUpdateDeviceMatrix(Matrix44f*, const Matrix44f&, Graphics::DevicePosture) const;
    virtual void doScreenPosToCameraPosTo(Vector3f*, const Vector3f&) const = 0;

    void updateMatrixImpl_() const;
    const Matrix44f& getDeviceProjectionMatrix() const;

private:
    mutable bool mDirty;
    mutable bool mDeviceDirty;
    Matrix44f mMatrix;
    Matrix44f mDeviceMatrix;
    Graphics::DevicePosture mDevicePosture;
    f32 mDeviceZScale;
    f32 mDeviceZOffset;
};

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
    u32 getProjectionType() const override;

    void set(float near, float far, float fovy_rad, float aspect);
    void doUpdateMatrix(Matrix44f* mtx) const override;
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
    float mFovyRad;
    float mFovySin;
    float mFovyCos;
    float mFovyTan;
    float mAspect;
    Vector2f mOffset;
};

}  // namespace sead

#endif  // SEAD_PROJECTION_H_
