#pragma once

#include <gfx/seadCamera.h>
#include <gfx/seadColor.h>
#include <gfx/seadDrawContext.h>
#include <gfx/seadProjection.h>
#include <heap/seadDisposer.h>
#include "nvn/nvn.h"
#include "thread/seadAtomic.h"

namespace sead
{
class UniformBlockBuffer
{
public:
    void swap(u32 temp, u32 size)
    {
        _4 = temp;
        if (temp + size < size)
        {
            u32 mod = temp % size;
            _4 = mod;
            _0 = mod;
        }
    }

    u32 get_0() const { return _0; }

private:
    Atomic<u32> _0, _4;
};

// unknown contents and size
class FontBase
{
public:
    virtual ~FontBase();

    virtual float getHeight() const = 0;
    virtual float getWidth() const = 0;
    virtual float getCharWidth(char16_t c) const = 0;
    virtual u32 getEncoding() const = 0;
    virtual u32 getMaxDrawNum() const = 0;
    virtual void begin(DrawContext* ctx) const = 0;
    virtual void end(DrawContext* ctx) const = 0;
    virtual void print(DrawContext* ctx, const Projection& proj, const Camera& cam,
                       const Matrix34f& mtx, const Color4f& color, const void* text,
                       int len) const = 0;
};

}  // namespace sead
