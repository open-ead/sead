#pragma once

#include <gfx/seadFrameBuffer.h>
#include <gfx/seadGraphics.h>
#include <gfx/seadProjection.h>

namespace sead
{
class Camera;
class Projection;

template <typename T>
class Ray;

class Viewport : public BoundBox2f
{
    SEAD_RTTI_BASE(Viewport)
public:
    Viewport();
    Viewport(float left, float top, float right, float bottom);
    explicit Viewport(const BoundBox2f& parent);
    explicit Viewport(const LogicalFrameBuffer& buffer);
    virtual ~Viewport() = default;

    void setByFrameBuffer(const LogicalFrameBuffer& buffer);

    void getOnFrameBufferPos(Vector2f* out, const LogicalFrameBuffer& buffer) const;
    void getOnFrameBufferSize(Vector2f* out, const LogicalFrameBuffer& buffer) const;

    void apply(DrawContext*, const LogicalFrameBuffer& buffer) const;
    void applyViewport(DrawContext* context, const LogicalFrameBuffer& buffer) const;
    void applyScissor(DrawContext* context, const LogicalFrameBuffer& buffer) const;

    void project(Vector2f*, const Vector3f&) const;
    void project(Vector2f*, const Vector2f&) const;
    void unproject(Vector3f*, const Vector2f&, const Projection&, const Camera&) const;
    void unprojectRay(Ray<Vector3f>*, const Vector2f&, const Projection&, const Camera&) const;

private:
    Graphics::DevicePosture mDevicePosture = Graphics::getDefaultDevicePosture();
    Vector2f mDepthBounds = Vector2f(0.0f, 1.0f);
};
#ifdef cafe
static_assert(sizeof(Viewport) == 0x18, "sead::Viewport size mismatch");
#endif  // cafe

}  // namespace sead
