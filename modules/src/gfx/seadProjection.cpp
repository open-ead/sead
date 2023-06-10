#include <gfx/seadProjection.h>

namespace sead
{

Projection::Projection()
{
    markDirty();
    markDeviceDirty();
    mDevicePosture = Graphics::DevicePosture::cDevicePosture_Same;  // Global
    mDeviceZScale = GLOBAL_DEVICE_Z_SCALE;                          // Global
    mDeviceZOffset = 1.0;                                           // Some Global Var.
}

void Projection::updateMatrixImpl_() const
{
    if (mDirty)
    {
        doUpdateMatrix(const_cast<Matrix44f*>(&mMatrix));
        mDirty = false;

        mDeviceDirty = true;
        doUpdateDeviceMatrix(const_cast<Matrix44f*>(&mDeviceMatrix), mMatrix, mDevicePosture);
        mDeviceDirty = false;
    }
    else if (mDeviceDirty)
    {
        doUpdateDeviceMatrix(const_cast<Matrix44f*>(&mDeviceMatrix), mMatrix, mDevicePosture);
        mDeviceDirty = false;
    }
}

const Matrix44f& Projection::getProjectionMatrix() const
{
    if (!mDirty)
    {
        if (!mDeviceDirty)
        {
            return mMatrix;
        }
    }
    else
    {
        doUpdateMatrix(const_cast<Matrix44f*>(&mMatrix));
        mDirty = false;
        mDeviceDirty = true;
    }

    doUpdateDeviceMatrix(const_cast<Matrix44f*>(&mDeviceMatrix), mMatrix, mDevicePosture);
    mDeviceDirty = false;
    return mMatrix;
}

const Matrix44f& Projection::getDeviceProjectionMatrix() const
{
    updateMatrixImpl_();
    return mDeviceMatrix;
}

void Projection::doUpdateDeviceMatrix(Matrix44f* deviceMatrix, const Matrix44f& projMatrix,
                                      Graphics::DevicePosture posture) const
{
    /* What this does:
    1. Copy the data into the device matrix
    2. Modify the device matrix based on the device orientation
        a.
    3. Scale the Z axis by the Device Z scale and include the zOffset.

    */
    *deviceMatrix = projMatrix;
    switch (posture)
    {
    case Graphics::DevicePosture::cDevicePosture_RotateRight:
    case Graphics::DevicePosture::cDevicePosture_RotateLeft:
    case Graphics::DevicePosture::cDevicePosture_RotateHalfAround:
    case Graphics::DevicePosture::cDevicePosture_FlipX:
    case Graphics::DevicePosture::cDevicePosture_FlipY:
    default:
        break;
    }
}

// void sead::Projection::someFunction(Camera* camera)
// {
//     doScreenPosToCameraPosTo(Vector3f*, const Vector3f&);
//     camera.Camera(); // ???
// }

// Intentional empty implementation
void Projection::updateAttributesForDirectProjection() {}

}  // namespace sead
