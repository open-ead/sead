#pragma once

#include "devenv/seadFontMgr.h"

namespace sead
{
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
                              u64 font_size, const void* table_binary, u32 unk3);
    void swapUniformBlockBuffer();
    u32 searchCharIndexFormCharCode_(u32 code) const;

private:
    NVNprogram mNvnProgram;
    NVNtexture mNvnTexture;
    NVNtextureHandle mNvnTextureHandle;
    NVNmemoryPool mPool1, mPool2, mPool3;
    NVNbuffer mBuffer1;
    u32 mBuffer1Size = 0;
    const void* mFileData = nullptr;
    NVNbuffer mBuffer2;
    void* mBuffer2Map = nullptr;
    UniformBlockBuffer mUniformBlockBuffer;
    bool _530 = false;
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
    UniformBlockBuffer mUniformBlockBuffer;
    bool _528 = 0;
};
static_assert(sizeof(DebugFontMgrNvn) == 0x530);
}  // namespace sead
