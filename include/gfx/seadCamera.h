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

    virtual void doUpdateMatrix(Matrix34f* dst) const = 0;

    void getWorldPosByMatrix(Vector3f* dst) const;
    void getLookVectorByMatrix(Vector3f* dst) const;
    void getRightVectorByMatrix(Vector3f* dst) const;
    void getUpVectorByMatrix(Vector3f* dst) const;

    void worldPosToCameraPosByMatrix(Vector3f* dst, const Vector3f& world_pos) const;
    void cameraPosToWorldPosByMatrix(Vector3f* dst, const Vector3f& camera_pos) const;

    void projectByMatrix(Vector2f* dst, const Vector3f& world_pos, const Projection& projection,
                         const Viewport& viewport) const;
    void unprojectRayByMatrix(Ray<Vector3f>* dst, const Vector3f& camera_pos) const;

    Matrix34f& getMatrix() { return mMatrix; }
    const Matrix34f& getMatrix() const { return mMatrix; }

    void updateViewMatrix() { doUpdateMatrix(&mMatrix); }

private:
    Matrix34f mMatrix = Matrix34f::ident;
};

class LookAtCamera : public Camera
{
    SEAD_RTTI_OVERRIDE(LookAtCamera, Camera)
public:
    LookAtCamera() = default;
    LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up);
    ~LookAtCamera() override;

    void doUpdateMatrix(Matrix34f* dst) const override;

    Vector3f& getPos() { return mPos; }
    Vector3f& getAt() { return mAt; }
    Vector3f& getUp() { return mUp; }
    const Vector3f& getPos() const { return mPos; }
    const Vector3f& getAt() const { return mAt; }
    const Vector3f& getUp() const { return mUp; }

private:
    Vector3f mPos = {0.0f, 0.0f, 10.0f};
    Vector3f mAt = {0.0f, 0.0f, 0.0f};
    Vector3f mUp = {0.0f, 1.0f, 0.0f};
};

class DirectCamera : public Camera
{
    SEAD_RTTI_OVERRIDE(DirectCamera, Camera)
public:
    virtual ~DirectCamera();

    void doUpdateMatrix(Matrix34f* dst) const override;

private:
    Matrix34f mDirectMatrix = Matrix34f::ident;
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
