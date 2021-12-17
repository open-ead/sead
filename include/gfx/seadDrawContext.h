#pragma once

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

private:
    u32 _8;
    u32 _C;
    u64 _10;
    u8 _18[0xC0 - 0x18];
    u64 _C0;
    u64 _C8;
    u64 _D0;
    u64 _D8;
    u64 _E0;
    u64 _E8;
};

}  // namespace sead
