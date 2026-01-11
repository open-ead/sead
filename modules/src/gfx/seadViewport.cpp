#include <gfx/seadCamera.h>
#include <gfx/seadDrawContext.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include "math/seadBoundBox.h"
#include "nn/gfx/gfx_StateInfo.h"

namespace sead
{

Viewport::Viewport()
{
    setUndef();
}

Viewport::Viewport(f32 left, f32 top, f32 sizeX, f32 sizeY)
    : BoundBox2f(left, top, left + sizeX, top + sizeY)
{
}
Viewport::Viewport(const LogicalFrameBuffer& frame_buffer)
{
    setByFrameBuffer(frame_buffer);
}

void Viewport::setByFrameBuffer(const LogicalFrameBuffer& frame_buffer)
{
    switch (mDevicePosture)
    {
    case Graphics::cDevicePosture_Same:
    case Graphics::cDevicePosture_FlipX:
    case Graphics::cDevicePosture_FlipY:
    case Graphics::cDevicePosture_FlipXY:
        set(0.0f, 0.0f, frame_buffer.getVirtualSize().x, frame_buffer.getVirtualSize().y);
        break;
    case Graphics::cDevicePosture_RotateRight:
    case Graphics::cDevicePosture_RotateLeft:
        set(0.0f, 0.0f, frame_buffer.getVirtualSize().y, frame_buffer.getVirtualSize().x);
        break;
    default:;
        // SEAD_ASSERT_MSG(false, "Undefined DevicePosture(%d)", s32(mDevicePos));
    }
}

void Viewport::getOnFrameBufferPos(Vector2f* dst, const LogicalFrameBuffer& fb) const
{
    *dst = getMin();

    switch (mDevicePosture)
    {
    case Graphics::cDevicePosture_Same:
        break;
    case Graphics::cDevicePosture_RotateRight:
    {
        f32 y = (fb.getVirtualSize().y - getSizeX()) - dst->x;
        dst->set(dst->y, y);
    }
    break;
    case Graphics::cDevicePosture_RotateLeft:
    {
        f32 x = (fb.getVirtualSize().x - getSizeY()) - dst->y;
        dst->set(x, dst->x);
    }
    break;
    case Graphics::cDevicePosture_FlipXY:
    {
        f32 x = (fb.getVirtualSize().x - getSizeX()) - dst->x;
        f32 y = (fb.getVirtualSize().y - getSizeY()) - dst->y;
        dst->set(x, y);
    }
    break;
    case Graphics::cDevicePosture_FlipX:
    {
        f32 x = (fb.getVirtualSize().x - getSizeX()) - dst->x;
        dst->set(x, dst->y);
    }
    break;
    case Graphics::cDevicePosture_FlipY:
    {
        f32 y = (fb.getVirtualSize().y - getSizeY()) - dst->y;
        dst->set(dst->x, y);
    }
    break;
    default:;
        // SEAD_ASSERT_MSG(false, "Undefined DevicePosture(%d)", s32(mDevicePos));
    }

    dst->x /= fb.getVirtualSize().x;
    dst->y /= fb.getVirtualSize().y;
    dst->x *= fb.getPhysicalArea().getSizeX();
    dst->y *= fb.getPhysicalArea().getSizeY();
    dst->x = dst->x + fb.getPhysicalArea().getMin().x;
    dst->y = dst->y + fb.getPhysicalArea().getMin().y;
}

void Viewport::getOnFrameBufferSize(Vector2f* dst, const LogicalFrameBuffer& fb) const
{
    dst->set(getSizeX(), getSizeY());

    switch (mDevicePosture)
    {
    case Graphics::cDevicePosture_Same:
    case Graphics::cDevicePosture_FlipX:
    case Graphics::cDevicePosture_FlipY:
    case Graphics::cDevicePosture_FlipXY:
        break;
    case Graphics::cDevicePosture_RotateRight:
    case Graphics::cDevicePosture_RotateLeft:
        dst->set(dst->y, dst->x);
        break;
    default:;
        // SEAD_ASSERT_MSG(false, "Undefined DevicePosture(%d)", s32(mDevicePos));
    }

    dst->x /= fb.getVirtualSize().x;
    dst->y /= fb.getVirtualSize().y;
    dst->x *= fb.getPhysicalArea().getSizeX();
    dst->y *= fb.getPhysicalArea().getSizeY();
}

void Viewport::apply(DrawContext* context, const LogicalFrameBuffer& buffer) const
{
    sead::Vector2f real_pos;
    getOnFrameBufferPos(&real_pos, buffer);

    sead::Vector2f real_size;
    getOnFrameBufferSize(&real_size, buffer);

    SEAD_ASSERT(buffer.getPhysicalArea().isInside(real_pos) &&
                buffer.getPhysicalArea().isInside(real_pos + real_size));

    real_pos.y = (buffer.getPhysicalArea().getSizeY() - real_size.y) - real_pos.y;

    sead::Graphics::instance()->setViewportRealPosition(real_pos.x, real_pos.y, real_size.x,
                                                        real_size.y);
    sead::Graphics::instance()->setScissorRealPosition(real_pos.x, real_pos.y, real_size.x,
                                                       real_size.y);
    context->getCommandBuffer()->SetDepthBounds(mDepthBounds.x, mDepthBounds.y);
}

void Viewport::applyViewport(DrawContext* context, const LogicalFrameBuffer& buffer) const
{
    sead::Vector2f real_pos;
    getOnFrameBufferPos(&real_pos, buffer);

    sead::Vector2f real_size;
    getOnFrameBufferSize(&real_size, buffer);

    SEAD_ASSERT(buffer.getPhysicalArea().isInside(real_pos) &&
                buffer.getPhysicalArea().isInside(real_pos + real_size));

    real_pos.y = (buffer.getPhysicalArea().getSizeY() - real_size.y) - real_pos.y;
    // real_pos.x = (buffer.getPhysicalArea().getSizeX() - real_size.x) - real_pos.x;

    sead::Graphics::instance()->setViewportRealPosition(real_pos.x, real_pos.y, real_size.x,
                                                        real_size.y);
}

void Viewport::applyScissor(DrawContext* context, const LogicalFrameBuffer& buffer) const
{
    sead::Vector2f real_pos;
    getOnFrameBufferPos(&real_pos, buffer);

    sead::Vector2f real_size;
    getOnFrameBufferSize(&real_size, buffer);

    SEAD_ASSERT(buffer.getPhysicalArea().isInside(real_pos) &&
                buffer.getPhysicalArea().isInside(real_pos + real_size));

    real_pos.y = (buffer.getPhysicalArea().getSizeY() - real_size.y) - real_pos.y;

    sead::Graphics::instance()->setScissorRealPosition(real_pos.x, real_pos.y, real_size.x,
                                                       real_size.y);
}

void Viewport::project(Vector2f* aVec, const Vector3f& bVec) const
{
    aVec->x = getHalfSizeX() * bVec.x;
    aVec->y = getHalfSizeY() * bVec.y;
}

void Viewport::project(Vector2f* aVec, const Vector2f& bVec) const
{
    aVec->x = getHalfSizeX() * bVec.x;
    aVec->y = getHalfSizeY() * bVec.y;
}

void Viewport::unproject(Vector3f* dst, const Vector2f& some2Vec, const Projection& projection,
                         const Camera& camera) const
{
    Vector3f screenPos;
    screenPos.x = some2Vec.x / getHalfSizeX();
    screenPos.y = some2Vec.y / getHalfSizeY();
    screenPos.z = 0.0f;
    projection.unproject(dst, screenPos, camera);
}

void Viewport::unprojectRay(Ray<Vector3f>* dst, const Vector2f& some2Vec,
                            const Projection& projection, const Camera& camera) const
{
    Vector3f screenPos;
    screenPos.x = some2Vec.x / getHalfSizeX();
    screenPos.y = some2Vec.y / getHalfSizeY();
    screenPos.z = 0.0f;
    projection.unprojectRay(dst, screenPos, camera);
}

}  // namespace sead
