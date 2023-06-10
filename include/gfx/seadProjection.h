#ifndef SEAD_PROJECTION_H_
#define SEAD_PROJECTION_H_

#include <basis/seadTypes.h>
#include <gfx/seadGraphics.h>
#include <gfx/seadViewport.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

#define GLOBAL_DEVICE_Z_SCALE 1.0

namespace sead
{
class Projection
{
    SEAD_RTTI_BASE(Projection)

public:
    enum ProjectionType
    {
        cPerspectiveProjection = 0,
        cOrthoProjection = 1,
        cDirectProjection = 2,
    };

    Projection();
    virtual ~Projection() = default;

    void markDirty() { mDirty = true; };
    void markDeviceDirty() { mDeviceDirty = true; };
    virtual float getNear() const = 0;
    virtual float getFar() const = 0;
    virtual float getFovy() const = 0;
    virtual float getAspect() const = 0;
    virtual void getOffset(Vector2f* offset) const = 0;
    virtual void updateAttributesForDirectProjection();
    virtual ProjectionType getProjectionType() const = 0;
    virtual void doUpdateMatrix(Matrix44f* mtx) const = 0;
    virtual void doUpdateDeviceMatrix(Matrix44f*, const Matrix44f&, Graphics::DevicePosture) const;
    virtual void doScreenPosToCameraPosTo(Vector3f* screen, const Vector3f& cam) const;
    virtual void someFunction(Camera* camera) const;
    virtual void project(Vector2f* aVec, Vector3f* bVec) const;
    virtual void unproject(Vector3f*, float, Camera*) const = 0;

    void updateMatrixImpl_() const;
    const Matrix44f& getProjectionMatrix() const;
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

}  // namespace sead

#endif  // SEAD_PROJECTION_H_
