#ifndef SEAD_PROJECTION_H_
#define SEAD_PROJECTION_H_

#include <basis/seadTypes.h>
#include <gfx/seadGraphics.h>
#include <gfx/seadViewport.h>
#include <math/seadBoundBox.h>
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

    virtual f32 getNear() const = 0;
    virtual f32 getFar() const = 0;
    virtual f32 getFovy() const = 0;
    virtual f32 getAspect() const = 0;
    virtual void getOffset(Vector2f* offset) const = 0;
    virtual void updateAttributesForDirectProjection();
    virtual u32 getProjectionType() const = 0;
    virtual void doUpdateMatrix(Matrix44f* mtx) const = 0;
    virtual void doUpdateDeviceMatrix(Matrix44f*, const Matrix44f&, Graphics::DevicePosture) const;
    virtual void doScreenPosToCameraPosTo(Vector3f*, const Vector3f&) const = 0;

    void updateMatrixImpl_() const;
    const Matrix44f& getDeviceProjectionMatrix() const;

    void setDirty() { mDirty = true; }
    void setDeviceDirty() { mDeviceDirty = true; }

    void setDevicePosture(Graphics::DevicePosture pose)
    {
        mDevicePosture = pose;
        setDeviceDirty();
    }

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
    PerspectiveProjection(f32 near, f32 far, f32 fovy_rad, f32 aspect);
    ~PerspectiveProjection() override;

    f32 getNear() const override;
    f32 getFar() const override;
    f32 getFovy() const override;
    f32 getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;
    u32 getProjectionType() const override;

    void set(f32 near, f32 far, f32 fovy_rad, f32 aspect);
    void doUpdateMatrix(Matrix44f* mtx) const override;
    void setFovx(f32);
    void createDividedProjection(PerspectiveProjection* projection, s32, s32, s32, s32);
    f32 getTop() const;
    f32 getBottom() const;
    f32 getLeft() const;
    f32 getRight() const;
    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

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

private:
    f32 mNear;
    f32 mFar;
    f32 mFovyRad;
    f32 mFovySin;
    f32 mFovyCos;
    f32 mFovyTan;
    f32 mAspect;
    Vector2f mOffset;
};

class OrthoProjection : public Projection
{
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
    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};

class FrustumProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(FrustumProjection, Projection)

public:
    FrustumProjection();
    FrustumProjection(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right);
    FrustumProjection(f32 near, f32 far, const BoundBox2f& boundBox);
    ~FrustumProjection() override;

    f32 getNear() const override;
    f32 getFar() const override;
    f32 getFovy() const override;
    f32 getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    f32 getOffsetX() const;
    f32 getOffsetY() const;
    u32 getProjectionType() const override;

    void doUpdateMatrix(Matrix44f* mtx) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;
    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);
    void setBoundBox(BoundBox2f& boundBox);
    void createDividedProjection(FrustumProjection* out, s32, s32, s32, s32) const;
    void setFovyAspectOffset(f32 fovy, f32 aspect, const Vector2f& offset);

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};

class DirectProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(DirectProjection, Projection)

public:
    DirectProjection();
    DirectProjection(const Matrix44f& mtx, Graphics::DevicePosture posture);
    ~DirectProjection() override;

    void setProjectionMatrix(const Matrix44f& mtx, Graphics::DevicePosture posture);
    f32 getNear() const override;
    f32 getFar() const override;
    f32 getFovy() const override;
    f32 getAspect() const override;
    void getOffset(Vector2f* offset) const override;
    void updateAttributesForDirectProjection() override;
    void doUpdateMatrix(Matrix44f* mtx) const override;
    void doScreenPosToCameraPosTo(Vector3f* cameraPos, const Vector3f& screenPos) const override;
    u32 getProjectionType() const override;

private:
    Matrix44f mProjectionMatrix;
    f32 mNear;
    f32 mFar;
    f32 mFovy;
    f32 mAspect;
    Vector2f mOffset;
    bool _f0;
};

}  // namespace sead

#endif  // SEAD_PROJECTION_H_
