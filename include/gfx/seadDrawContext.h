#pragma once

#include <nvn/nvn.h>
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

    NVNcommandBuffer* getNvnCommandBuffer() const { return nvnCommandBuffer; }

private:
    u32 _8;
    u32 _c;
    u64 _10;
    u8 _18[0xC0 - 0x18];
    NVNcommandBuffer* nvnCommandBuffer;
    u64 _c8;
    u64 _d0;
    u64 _d8;
    u64 _e0;
    u64 _e8;
};

}  // namespace sead
