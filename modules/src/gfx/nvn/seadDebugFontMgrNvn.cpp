#include "gfx/nvn/seadDebugFontMgrNvn.h"
#include "filedevice/seadFileDevice.h"
#include "filedevice/seadFileDeviceMgr.h"
#include "gfx/nin/seadGraphicsNvn.h"
#include "gfx/seadGraphics.h"
#include "nn/os.h"
#include "nvn/nvn.h"
#include "nvn/nvn_FuncPtrInline.h"

namespace sead
{
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

// SMO
// NON_MATCHING: Stack
void DebugFontMgrJis1Nvn::initializeFromBinary(Heap* heap, void* shader_binary, u64 shader_size,
                                               void* font_binary, u64 font_size,
                                               const void* table_binary, u32 unk3)
{
    struct ShaderBinaryHeader
    {
        u32 offset1;
        u32 offset2;
        u32 shaderDataOffset;
        u32 offset4;
    };

    mFileData = table_binary;
    mBuffer1Size = unk3;

    NVNdevice* device = static_cast<GraphicsNvn*>(sead::Graphics::instance())->getNvnDevice();

    nvnProgramInitialize(&mNvnProgram, device);

    NVNmemoryPoolBuilder poolBuilder;
    nvnMemoryPoolBuilderSetDefaults(&poolBuilder);
    nvnMemoryPoolBuilderSetDevice(&poolBuilder, device);
    nvnMemoryPoolBuilderSetFlags(&poolBuilder, NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED |
                                                   NVN_MEMORY_POOL_FLAGS_GPU_CACHED |
                                                   NVN_MEMORY_POOL_FLAGS_SHADER_CODE);

    size_t paddedShaderSize = (shader_size + 0xFFF) & ~0xFFF;

    nvnMemoryPoolBuilderSetStorage(&poolBuilder, shader_binary, paddedShaderSize);
    nvnMemoryPoolInitialize(&mPool2, &poolBuilder);

    NVNbufferBuilder bufferBuilder;
    nvnBufferBuilderSetDevice(&bufferBuilder, device);
    nvnBufferBuilderSetDefaults(&bufferBuilder);
    nvnBufferBuilderSetStorage(&bufferBuilder, &mPool2, 0, paddedShaderSize);
    nvnBufferInitialize(&mBuffer1, &bufferBuilder);

    NVNbufferAddress bufferAddress = nvnBufferGetAddress(&mBuffer1);

    ShaderBinaryHeader* header = reinterpret_cast<ShaderBinaryHeader*>(shader_binary);
    NVNshaderData data[2]{
        {bufferAddress + header->shaderDataOffset,
         reinterpret_cast<void*>(uintptr_t(shader_binary) + ptrdiff_t(header->offset1))},
        {bufferAddress + header->offset4,
         reinterpret_cast<void*>(uintptr_t(shader_binary) + ptrdiff_t(header->offset2))}};
    nvnProgramSetShaders(&mNvnProgram, sizeof(data) / sizeof(NVNshaderData), data);
    static size_t sPaddedBuffer1Size = (size_t(mBuffer1Size + 0x200) + 0xFFF) & ~0xFFF;

    nvnMemoryPoolBuilderSetDefaults(&poolBuilder);
    nvnMemoryPoolBuilderSetDevice(&poolBuilder, device);
    nvnMemoryPoolBuilderSetFlags(&poolBuilder, NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED |
                                                   NVN_MEMORY_POOL_FLAGS_GPU_CACHED);
    void* storage = new (heap) char[sPaddedBuffer1Size];
    nvnMemoryPoolBuilderSetStorage(&poolBuilder, storage, sPaddedBuffer1Size);
    nvnMemoryPoolInitialize(&mPool3, &poolBuilder);

    nvnBufferBuilderSetDefaults(&bufferBuilder);
    nvnBufferBuilderSetDevice(&bufferBuilder, device);
    nvnBufferBuilderSetStorage(&bufferBuilder, &mPool3, 0, mBuffer1Size);
    nvnBufferInitialize(&mBuffer2, &bufferBuilder);
    mBuffer2Map = nvnBufferMap(&mBuffer2);

    NVNtextureBuilder texBuilder;
    nvnTextureBuilderSetDefaults(&texBuilder);
    nvnTextureBuilderSetDevice(&texBuilder, device);
    nvnTextureBuilderSetTarget(&texBuilder, NVN_TEXTURE_TARGET_2D);
    nvnTextureBuilderSetFormat(&texBuilder, NVN_FORMAT_RGTC1_UNORM);
    nvnTextureBuilderSetSize2D(&texBuilder, 1280, 1298);
    nvnTextureBuilderSetPackagedTextureData(
        &texBuilder, reinterpret_cast<void*>(uintptr_t(font_binary) + 0x200));

    nvnMemoryPoolBuilderSetDefaults(&poolBuilder);
    nvnMemoryPoolBuilderSetDevice(&poolBuilder, device);
    nvnMemoryPoolBuilderSetFlags(&poolBuilder, NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS |
                                                   NVN_MEMORY_POOL_FLAGS_GPU_CACHED);
    nvnMemoryPoolBuilderSetStorage(&poolBuilder, font_binary, (font_size + 0xFFF) & ~0xFFF);
    nvnMemoryPoolInitialize(&mPool1, &poolBuilder);

    nvnTextureBuilderSetStorage(&texBuilder, &mPool1, 0x200);
    nvnTextureInitialize(&mNvnTexture, &texBuilder);

    CriticalSection* criticalSection2 =
        static_cast<GraphicsNvn*>(sead::Graphics::instance())->getCriticalSection2();
    criticalSection2->lock();
    int newTextureId = static_cast<GraphicsNvn*>(sead::Graphics::instance())->getNewTextureId();
    nvnTexturePoolRegisterTexture(
        static_cast<GraphicsNvn*>(sead::Graphics::instance())->getTexturePool(), newTextureId,
        &mNvnTexture, nullptr);
    criticalSection2->unlock();
    mNvnTextureHandle = nvnDeviceGetTextureHandle(
        device, newTextureId,
        static_cast<GraphicsNvn*>(sead::Graphics::instance())->getTextureSamplerID());
}

void DebugFontMgrJis1Nvn::swapUniformBlockBuffer()
{
    mUniformBlockBuffer.swap(mUniformBlockBuffer.get_0(), mBuffer1Size);
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
    nvnCommandBufferBindProgram(ctx->getCommandBuffer()->ToData()->pNvnCommandBuffer, &mNvnProgram,
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
