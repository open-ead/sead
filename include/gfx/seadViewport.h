#ifndef SEAD_VIEWPORT_H_
#define SEAD_VIEWPORT_H_

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
    Viewport(f32 left, f32 top, f32 sizeX, f32 sizeY);
    // explicit Viewport(const BoundBox2f& box);
    explicit Viewport(const LogicalFrameBuffer& frame_buffer);
    virtual ~Viewport() = default;

    void setByFrameBuffer(const LogicalFrameBuffer& frame_buffer);

    void getOnFrameBufferPos(Vector2f* dst, const LogicalFrameBuffer& fb) const;
    void getOnFrameBufferSize(Vector2f* dst, const LogicalFrameBuffer& fb) const;

    void apply(DrawContext* context, const LogicalFrameBuffer& frame_buffer) const;
    void applyViewport(DrawContext* context, const LogicalFrameBuffer& buffer) const;
    void applyScissor(DrawContext* context, const LogicalFrameBuffer& buffer) const;
    void project(Vector2f*, const Vector3f&) const;
    void project(Vector2f*, const Vector2f&) const;
    void unproject(Vector3f*, const Vector2f&, const Projection&, const Camera&) const;
    void unproject(Ray<Vector3f>*, const Vector2f&, const Projection&, const Camera&) const;

private:
    Graphics::DevicePosture mDevicePos = Graphics::getDefaultDevicePosture();
    Vector2f mDepthBounds = Vector2f(0.0f, 1.0f);
};
#ifdef cafe
static_assert(sizeof(Viewport) == 0x18, "sead::Viewport size mismatch");
#endif  // cafe

}  // namespace sead

#endif  // SEAD_VIEWPORT_H_
