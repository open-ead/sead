#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead
{
class OrthoProjection;
class Projection;
class Viewport;
template <typename T>
class Ray;

class Camera
{
    SEAD_RTTI_BASE(Camera)

public:
    Camera() = default;
    virtual ~Camera();

    virtual void doUpdateMatrix(Matrix34f* mtx) const = 0;

    void getWorldPosByMatrix(Vector3f*) const;
    void getLookVectorByMatrix(Vector3f*) const;
    void getRightVectorByMatrix(Vector3f*) const;
    void getUpVectorByMatrix(Vector3f*) const;
    void worldPosToCameraPosByMatrix(Vector3f*, const Vector3f&) const;
    void cameraPosToWorldPosByMatrix(Vector3f*, const Vector3f&) const;
    void projectByMatrix(Vector2f*, const Vector3f&, const Projection&, const Viewport&) const;
    void unprojectRayByMatrix(Ray<Vector3f>*, const Vector3f&) const;

private:
    Matrix34f mMatrix = Matrix34f::ident;
};

class LookAtCamera : public Camera
{
    SEAD_RTTI_OVERRIDE(LookAtCamera, Camera)
public:
    LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up);
    ~LookAtCamera() override;

    Vector3f& getPos() { return mPos; }
    Vector3f& getAt() { return mAt; }
    Vector3f& getUp() { return mUp; }
    const Vector3f& getPos() const { return mPos; }
    const Vector3f& getAt() const { return mAt; }
    const Vector3f& getUp() const { return mUp; }

    void doUpdateMatrix(Matrix34f* mtx) const override;

private:
    Vector3f mPos;
    Vector3f mAt;
    Vector3f mUp;
};

class DirectCamera : public Camera
{
    SEAD_RTTI_OVERRIDE(DirectCamera, Camera)
public:
    virtual ~DirectCamera();

    void doUpdateMatrix(Matrix34f* mtx) const override;

private:
    Matrix34f mMtx = Matrix34f::ident;
};

class OrthoCamera : public LookAtCamera
{
    SEAD_RTTI_OVERRIDE(OrthoCamera, LookAtCamera)
public:
    OrthoCamera();
    OrthoCamera(const Vector2f&, float);
    OrthoCamera(const OrthoProjection&);
    ~OrthoCamera() override;

    void setByOrthoProjection(const OrthoProjection&);
    void setRotation(float rotation);
};
}  // namespace sead
