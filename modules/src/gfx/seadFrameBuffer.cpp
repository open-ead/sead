#include <gfx/seadFrameBuffer.h>

namespace sead
{
FrameBuffer::~FrameBuffer() = default;

void FrameBuffer::clearMRT(DrawContext*, u32, const Color4f&) const {}

void FrameBuffer::bind(DrawContext* draw_context) const
{
    bindImpl_(draw_context);
}
}  // namespace sead
