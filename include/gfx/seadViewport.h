#pragma once

#include <gfx/seadDrawContext.h>
#include <gfx/seadFrameBuffer.h>
#include <math/seadVector.h>

namespace sead
{
template <typename T>
class Ray;

class Viewport : public BoundBox2f
{
public:
    Viewport();
    Viewport(float left, float top, float right, float bottom);
    Viewport(const BoundBox2f&);
    Viewport(const LogicalFrameBuffer&);
    virtual ~Viewport();

    void setByFrameBuffer(const LogicalFrameBuffer&);
    void apply(DrawContext*, const LogicalFrameBuffer&) const;
    void getOnFrameBufferPos(Vector2f*, const LogicalFrameBuffer&) const;
    void getOnFrameBufferSize(Vector2f*, const LogicalFrameBuffer&) const;
    void applyViewport(DrawContext*, const LogicalFrameBuffer&) const;
    void applyScissor(DrawContext*, const LogicalFrameBuffer&) const;
    void project(Vector2f*, const Vector3f&) const;
    void project(Vector2f*, const Vector2f&) const;
    void unproject(Vector3f*, const Vector2f&, const Projection&, const Camera&) const;
    void unproject(Ray<Vector3f>*, const Vector2f&, const Projection&, const Camera&) const;

private:
    s32 mDevicePosture;
};
}  // namespace sead
