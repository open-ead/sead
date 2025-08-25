#pragma once

#include <nn/gfx/gfx_CommandBuffer.h>
#include <nn/gfx/gfx_Types.h>
#include "basis/seadTypes.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class DrawContext
{
    SEAD_RTTI_BASE(DrawContext)
public:
    DrawContext();
    virtual ~DrawContext();

    nn::gfx::CommandBuffer* getCommandBuffer() { return &mCommandBuffer; }

private:
    nn::gfx::CommandBuffer mCommandBuffer;
};
static_assert(sizeof(DrawContext) == 0xF0);

}  // namespace sead
