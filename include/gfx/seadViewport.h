#pragma once

#include <gfx/seadFrameBuffer.h>
#include <gfx/seadGraphics.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
template <typename T>
class Ray;
class DrawContext;
class Projection;
class Camera;

class Viewport : public BoundBox2f
{
    SEAD_RTTI_BASE(Viewport)
public:
    Viewport();
    Viewport(float left, float top, float right, float bottom);
    explicit Viewport(const BoundBox2f& parent);
    explicit Viewport(LogicalFrameBuffer const& buffer);
    virtual ~Viewport() = default;

    void setByFrameBuffer(const LogicalFrameBuffer& buffer);
    void apply(DrawContext*, const LogicalFrameBuffer& buffer) const;
    void getOnFrameBufferPos(Vector2f* out, const LogicalFrameBuffer& buffer) const;
    void getOnFrameBufferSize(Vector2f* out, const LogicalFrameBuffer& buffer) const;
    void applyViewport(DrawContext* context, const LogicalFrameBuffer& buffer) const;
    void applyScissor(DrawContext* context, const LogicalFrameBuffer& buffer) const;
    void project(Vector2f*, const Vector3f&) const;
    void project(Vector2f*, const Vector2f&) const;
    void unproject(Vector3f*, const Vector2f&, const Projection&, const Camera&) const;
    void unproject(Ray<Vector3f>*, const Vector2f&, const Projection&, const Camera&) const;

private:
    inline void frameBufferHelper(const LogicalFrameBuffer& buffer);
    Graphics::DevicePosture mDevicePosture;
    float mMaybeMinX;
    float mMaybeMinY;
    float mMaybeMaxX;
    float mMaybeMaxY;
};
}  // namespace sead
