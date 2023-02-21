#pragma once

#include <gfx/seadCamera.h>
#include <gfx/seadColor.h>
#include <gfx/seadDrawContext.h>
#include <gfx/seadProjection.h>
#include <heap/seadDisposer.h>
#include "nvn/nvn.h"

namespace sead
{
// unknown contents and size
class FontBase
{
public:
    virtual ~FontBase();
};

class DebugFontMgrJis1Nvn : public FontBase
{
    SEAD_SINGLETON_DISPOSER(DebugFontMgrJis1Nvn)
public:
    DebugFontMgrJis1Nvn();
    ~DebugFontMgrJis1Nvn() override;

    void initialize(Heap* heap, const char* shader_path, const char* font_path,
                    const char* table_path, u32 unk);
    void initializeFromBinary(Heap* heap, void* shader_binary, u64 shader_size, void* font_binary,
                              u64 font_size, void* table_binary, u32 unk3);
    void swapUniformBlockBuffer();
    float getHeight() const;
    float getWidth() const;
    float getCharWidth(char16_t c) const;
    u32 getMaxDrawNum() const;
    void begin(DrawContext* ctx) const;
    void end(DrawContext* ctx) const;
    void print(DrawContext* ctx, const Projection& proj, const Camera& cam, const Matrix34f& mtx,
               const Color4f& color, const void* text, int len) const;
    u32 searchCharIndexFormCharCode_(u32 code) const;
    u32 getEncoding() const;

private:
    NVNprogram nvnProgram;
    u64 _e8[127];
    u32 _4f8 = 0;
    u64 _500 = 0;
    u64 _508[6];
    u64 _520 = 0;
    u32 _528 = 0;
    u32 _52c = 0;
    u8 _530 = 0;
};
static_assert(sizeof(DebugFontMgrJis1Nvn) == 0x538);

class DebugFontMgrNvn : public FontBase
{
    SEAD_SINGLETON_DISPOSER(DebugFontMgrNvn)
public:
    DebugFontMgrNvn();
    ~DebugFontMgrNvn() override;

    void initialize(Heap* heap, const char* shader_path, const char* font_path, u32 unk);
    void initializeFromBinary(Heap* heap, void* shader_binary, u64 shader_size, void* font_binary,
                              u64 font_size, u32 unk3);
    void swapUniformBlockBuffer();
    float getHeight() const;
    float getWidth() const;
    float getCharWidth(char16_t c) const;
    u32 getMaxDrawNum() const;
    void begin(DrawContext* ctx) const;
    void end(DrawContext* ctx) const;
    void print(DrawContext* ctx, const Projection& proj, const Camera& cam, const Matrix34f& mtx,
               const Color4f& color, const void* text, int len) const;
    u32 searchCharIndexFormCharCode_(u32 code) const;
    u32 getEncoding() const;

private:
    NVNprogram nvnProgram;
    u64 _e8[127];
    u32 _4F8 = 0;
    u64 _500[6];
    u64 _518 = 0;
    u32 _520 = 0;
    u32 _524 = 0;
    u8 _528 = 0;
};
static_assert(sizeof(DebugFontMgrNvn) == 0x530);

}  // namespace sead
