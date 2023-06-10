#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include "math/seadBoundBox.h"
#include "math/seadMathCalcCommon.h"

namespace sead
{

Viewport::Viewport()
{
    mMinX = 0.0;
    mMinY = 1.0;
}

Viewport::Viewport(float left, float top, float right, float bottom)
    : BoundBox2f(left, top, right, bottom)
{
    // float max = left + right;
    // float min = left;

    // if (max <= min)
    // {
    //     max = left;
    //     min = right;
    // }
    // this->mMaybeMinX = min;
    // this->mMaybeMaxX = max;

    // min = top;
    // max = bottom + top;
    // if (max <= min)
    // {
    //     max = top;
    //     min = bottom;
    // }
    // this->mMaybeMinY = min;
    // this->mMaybeMaxY = max;

    // // If L + R is smaller, then the left bound is the max X.
    // if ((left + right) <= left)
    // {
    //     this->mMaybeMinX = left + right;
    //     this->mMaybeMaxX = left;
    // }
    // else
    // {
    //     this->mMaybeMinX = left;
    //     this->mMaybeMaxX = left + right;
    // }

    // // Same deal for top and bottom
    // if ((top + bottom) <= top)
    // {
    //     this->mMaybeMinY = top + bottom;
    //     this->mMaybeMaxY = top;
    // }
    // else
    // {
    //     this->mMaybeMinY = top;
    //     this->mMaybeMaxY = top + bottom;
    // }
}

Viewport::Viewport(LogicalFrameBuffer const& buffer)
{
    setByFrameBuffer(buffer);
}

void Viewport::setByFrameBuffer(const LogicalFrameBuffer& buffer)
{
    mMinX = 0.0;
    mMinY = 1.0;

    // mDevicePosture = // Some Global device posture //
    if (mDevicePosture <= Graphics::DevicePosture::cDevicePosture_FlipY)
    {
        float minX;
        float maxX;
        float minY;
        float maxY;
        if (mDevicePosture == Graphics::DevicePosture::cDevicePosture_RotateLeft ||
            mDevicePosture == Graphics::DevicePosture::cDevicePosture_RotateHalfAround)
        {
            maxX = buffer.getVirtualSize().x;
            maxY = buffer.getVirtualSize().y;
        }
        else
        {
            maxY = buffer.getVirtualSize().x;
            maxX = buffer.getVirtualSize().y;
        }

        minY = fmin(maxY, (float)0.0);
        if (maxY < 0.0)
        {
            maxY = 0.0;
        }

        minX = fmin(maxX, (float)0.0);
        if (maxX < 0.0)
        {
            maxX = 0.0;
        }

        set(minX, minY, maxX, maxY);
    }
}

// void Viewport::apply(DrawContext*, const LogicalFrameBuffer& buffer) const {}

// void Viewport::getOnFrameBufferPos(Vector2f* out, const LogicalFrameBuffer& buffer) const
// {
//     switch (mDevicePosture)
//     {
//     case Graphics::DevicePosture::cDevicePosture_Same:
//     case Graphics::DevicePosture::cDevicePosture_RotateRight:
//     case Graphics::DevicePosture::cDevicePosture_RotateLeft:
//     case Graphics::DevicePosture::cDevicePosture_RotateHalfAround:
//     case Graphics::DevicePosture::cDevicePosture_FlipX:
//     case Graphics::DevicePosture::cDevicePosture_FlipY:
//     default:
//         break;
//     }
// }

void Viewport::getOnFrameBufferSize(Vector2f* out, const LogicalFrameBuffer& buffer) const
{
    out->x = getSizeX();
    out->y = getSizeY();

    if (mDevicePosture - 1 < Graphics::DevicePosture::cDevicePosture_RotateLeft)
    {
        out->x = getSizeY();
        out->y = getSizeX();
    }
    out->x = out->x / buffer.getVirtualSize().x;
    out->y = out->y / buffer.getVirtualSize().y;
    out->x = out->x * buffer.getPhysicalArea().getSizeX();
    out->y = out->y * buffer.getPhysicalArea().getSizeY();
}

// void Viewport::applyViewport(DrawContext* context, const LogicalFrameBuffer& buffer) const {}
// void Viewport::applyScissor(DrawContext* context, const LogicalFrameBuffer& buffer) const {}

void Viewport::project(Vector2f* aVec, const Vector2f& bVec) const
{
    aVec->x = (float)0.5 * (mMaxX - mMinX) * bVec.x;
    aVec->y = (float)0.5 * (mMaxY - mMinY) * bVec.y;
}

void Viewport::unproject(Vector3f* some3Vec, const Vector2f& some2Vec, const Projection& projection,
                         const Camera& camera) const
{
    float some_value;
    some_value = some2Vec.x / getHalfSizeX();
}
void Viewport::unproject(Ray<Vector3f>* ray, const Vector2f& someVec, const Projection& projection,
                         const Camera& camera) const
{
    float some_value;
    some_value = someVec.x / (getHalfSizeX());
    // projection.unproject(ray, some_value, camera);
}

}  // namespace sead
