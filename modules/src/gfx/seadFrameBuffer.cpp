#include <gfx/seadFrameBuffer.h>

namespace sead
{
LogicalFrameBuffer::~LogicalFrameBuffer() = default;

FrameBuffer::~FrameBuffer() = default;

void FrameBuffer::copyToDisplayBuffer(DrawContext*, const DisplayBuffer*) const {}

void FrameBuffer::clearMRT(DrawContext*, u32, const Color4f&) const {}

void FrameBuffer::bind(DrawContext* draw_context) const
{
    bindImpl_(draw_context);
}
}  // namespace sead
