#include "devenv/seadFontMgr.h"
#include "filedevice/seadFileDevice.h"
#include "filedevice/seadFileDeviceMgr.h"
#include "nvn/nvn.h"
#include "nvn/nvn_FuncPtrInline.h"

namespace sead
{
FontBase::~FontBase() = default;

SEAD_SINGLETON_DISPOSER_IMPL(DebugFontMgrJis1Nvn)
DebugFontMgrJis1Nvn::~DebugFontMgrJis1Nvn() = default;

DebugFontMgrJis1Nvn::DebugFontMgrJis1Nvn() = default;

void DebugFontMgrJis1Nvn::initialize(Heap* heap, const char* shader_path, const char* font_path,
                                     const char* table_path, u32 unk)
{
    FileDevice::LoadArg load_arg = {};
    load_arg.heap = heap;

    load_arg.path = table_path;
    void* table_binary = FileDeviceMgr::instance()->tryLoad(load_arg);

    load_arg.path = font_path;
    load_arg.alignment = 0x1000;
    load_arg.buffer_size_alignment = 0x1000;
    void* font_binary = FileDeviceMgr::instance()->tryLoad(load_arg);
    u32 font_size = load_arg.read_size;

    load_arg.path = shader_path;
    void* shader_binary = FileDeviceMgr::instance()->tryLoad(load_arg);
    u32 shader_size = load_arg.read_size;

    initializeFromBinary(heap, shader_binary, shader_size, font_binary, font_size, table_binary,
                         unk);
}

// missing initializeFromBinary

void DebugFontMgrJis1Nvn::swapUniformBlockBuffer()
{
    mUniformBlockBuffer.swap(mUniformBlockBuffer.get_0(), buffer1Size);
    _530 = false;
}

float DebugFontMgrJis1Nvn::getHeight() const
{
    return 16.0f;
}
float DebugFontMgrJis1Nvn::getWidth() const
{
    return 16.0f;
}
float DebugFontMgrJis1Nvn::getCharWidth(char16_t c) const
{
    return c < 0x7F ? 8.0f : 16.0f;
}
u32 DebugFontMgrJis1Nvn::getMaxDrawNum() const
{
    return 0x80;
}

void DebugFontMgrJis1Nvn::begin(DrawContext* ctx) const
{
    if (_530)
        return;
    nvnCommandBufferBindProgram(ctx->getCommandBuffer()->ToData()->pNvnCommandBuffer, &nvnProgram,
                                31);
}
void DebugFontMgrJis1Nvn::end(DrawContext*) const {}

// missing print and searchCharIndexFormCharCode_

u32 DebugFontMgrJis1Nvn::getEncoding() const
{
    return 2;
}

SEAD_SINGLETON_DISPOSER_IMPL(DebugFontMgrNvn)
DebugFontMgrNvn::~DebugFontMgrNvn() = default;

DebugFontMgrNvn::DebugFontMgrNvn() = default;

void DebugFontMgrNvn::initialize(Heap* heap, const char* shader_path, const char* font_path,
                                 u32 unk)
{
    FileDevice::LoadArg load_arg = {};

    load_arg.path = font_path;
    load_arg.heap = heap;
    load_arg.alignment = 0x1000;
    load_arg.buffer_size_alignment = 0x1000;
    void* font_binary = FileDeviceMgr::instance()->tryLoad(load_arg);
    u32 font_size = load_arg.read_size;

    load_arg.path = shader_path;
    void* shader_binary = FileDeviceMgr::instance()->tryLoad(load_arg);
    u32 shader_size = load_arg.read_size;

    initializeFromBinary(heap, shader_binary, shader_size, font_binary, font_size, unk);
}

// missing initializeFromBinary

void DebugFontMgrNvn::swapUniformBlockBuffer()
{
    mUniformBlockBuffer.swap(mUniformBlockBuffer.get_0(), buffer1Size);
    _528 = false;
}

float DebugFontMgrNvn::getHeight() const
{
    return 16.0f;
}
float DebugFontMgrNvn::getWidth() const
{
    return 8.0f;
}
float DebugFontMgrNvn::getCharWidth(char16_t) const
{
    return 8.0f;
}
u32 DebugFontMgrNvn::getMaxDrawNum() const
{
    return 0x80;
}

void DebugFontMgrNvn::begin(DrawContext* ctx) const
{
    if (_528)
        return;
    nvnCommandBufferBindProgram(ctx->getCommandBuffer()->ToData()->pNvnCommandBuffer, &nvnProgram,
                                NVN_SHADER_STAGE_ALL_GRAPHICS_BITS);
}
void DebugFontMgrNvn::end(DrawContext*) const {}

// missing print and searchCharIndexFormCharCode_

u32 DebugFontMgrNvn::getEncoding() const
{
    return 2;
}

}  // namespace sead
