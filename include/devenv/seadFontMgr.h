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

class DebugFontMgrJis1Nvn : public FontBase
{
    SEAD_SINGLETON_DISPOSER(DebugFontMgrJis1Nvn)
public:
    DebugFontMgrJis1Nvn();
    ~DebugFontMgrJis1Nvn() override;

    float getHeight() const override;
    float getWidth() const override;
    float getCharWidth(char16_t c) const override;
    u32 getEncoding() const override;
    u32 getMaxDrawNum() const override;
    void begin(DrawContext* ctx) const override;
    void end(DrawContext* ctx) const override;
    void print(DrawContext* ctx, const Projection& proj, const Camera& cam, const Matrix34f& mtx,
               const Color4f& color, const void* text, int len) const override;

    void initialize(Heap* heap, const char* shader_path, const char* font_path,
                    const char* table_path, u32 unk);
    void initializeFromBinary(Heap* heap, void* shader_binary, u64 shader_size, void* font_binary,
                              u64 font_size, void* table_binary, u32 unk3);
    void swapUniformBlockBuffer();
    u32 searchCharIndexFormCharCode_(u32 code) const;

private:
    NVNprogram nvnProgram;
    NVNtexture nvnTexture;
    NVNtextureHandle nvnTextureHandle;
    NVNmemoryPool pool1, pool2, pool3;
    NVNbuffer buffer1;
    u32 buffer1Size = 0;
    void* fileData = nullptr;
    NVNbuffer buffer2;
    void* buffer2Map = nullptr;
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

    float getHeight() const override;
    float getWidth() const override;
    float getCharWidth(char16_t c) const override;
    u32 getEncoding() const override;
    u32 getMaxDrawNum() const override;
    void begin(DrawContext* ctx) const override;
    void end(DrawContext* ctx) const override;
    void print(DrawContext* ctx, const Projection& proj, const Camera& cam, const Matrix34f& mtx,
               const Color4f& color, const void* text, int len) const override;

    void initialize(Heap* heap, const char* shader_path, const char* font_path, u32 unk);
    void initializeFromBinary(Heap* heap, void* shader_binary, u64 shader_size, void* font_binary,
                              u64 font_size, u32 unk3);
    void swapUniformBlockBuffer();
    u32 searchCharIndexFormCharCode_(u32 code) const;

private:
    NVNprogram nvnProgram;
    NVNtexture nvnTexture;
    NVNtextureHandle nvnTextureHandle;
    NVNmemoryPool pool1, pool2, pool3;
    NVNbuffer buffer1;
    u32 buffer1Size = 0;
    u32 _4e4;
    NVNbuffer buffer2;
    void* buffer2Map = nullptr;
    u32 _520 = 0;
    u32 _524 = 0;
    u8 _528 = 0;
};
static_assert(sizeof(DebugFontMgrNvn) == 0x530);

}  // namespace sead
