#pragma once

#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>
#include <thread/seadCriticalSection.h>
#include "nvn/nvn.h"

namespace sead
{
class DisplayBufferNvn;
enum class NVNdebugCallbackSource;
enum class NVNdebugCallbackType;
enum class NVNdebugCallbackSeverity;

class GraphicsNvn : public Graphics
{
public:
    class CreateArg;
    struct NvnDebugCallbackParam;

    GraphicsNvn(const CreateArg& arg);

    void initializeDrawLockContext(Heap*) override;
    void initializeImpl(Heap*) override;

    s32 getNewSamplerId();
    s32 getNewTextureId();

    void setDisplayBufferWindowCrop(s32, s32, s32, s32);
    void getDisplayBufferWindowCrop(s32*, s32*, s32*, s32*) const;

    void registerQueue(NVNqueue*);
    void registerDefaultCommandBuffer(NVNcommandBuffer*);
    void registerDisplayBufferNvn(DisplayBufferNvn*);
    void applyDeferredFinalizes();

    void nvnDebugCallback(NVNdebugCallbackSource, NVNdebugCallbackType, s32,
                          NVNdebugCallbackSeverity, const char*, void*);

    u64 convertGPUTimeStampToSystemTick(const NVNcounterData*);
    u32 convertNvnDebugLevel(u32);
    void setViewportImpl(f32, f32, f32, f32) override;
    void setScissorImpl(f32, f32, f32, f32) override;
    void setDepthTestEnableImpl(bool) override;
    void setDepthWriteEnableImpl(bool) override;
    void setDepthFuncImpl(Graphics::DepthFunc) override;
    void setVBlankWaitIntervalImpl(u32) override;
    void setCullingModeImpl(Graphics::CullingMode) override;
    void setBlendEnableImpl(bool) override;
    void setBlendEnableMRTImpl(u32, bool) override;
    void setBlendFactorImpl(Graphics::BlendFactor, Graphics::BlendFactor, Graphics::BlendFactor,
                            Graphics::BlendFactor) override;
    void setBlendFactorMRTImpl(u32, Graphics::BlendFactor, Graphics::BlendFactor,
                               Graphics::BlendFactor, Graphics::BlendFactor) override;
    void setBlendEquationImpl(Graphics::BlendEquation, Graphics::BlendEquation) override;
    void setBlendEquationMRTImpl(u32, Graphics::BlendEquation, Graphics::BlendEquation) override;
    void setBlendConstantColorImpl(sead::Color4f const&) override;
    void waitForVBlankImpl() override;
    void setColorMaskImpl(bool, bool, bool, bool) override;
    void setColorMaskMRTImpl(u32, bool, bool, bool, bool) override;
    void setAlphaTestEnableImpl(bool) override;
    void setAlphaTestFuncImpl(Graphics::AlphaFunc, f32) override;
    void setStencilTestEnableImpl(bool) override;
    void setStencilTestFuncImpl(Graphics::StencilFunc, s32, u32) override;
    void setStencilTestOpImpl(Graphics::StencilOp, Graphics::StencilOp,
                              Graphics::StencilOp) override;
    void setPolygonModeImpl(Graphics::PolygonMode, Graphics::PolygonMode) override;
    void setPolygonOffsetEnableImpl(bool, bool, bool) override;

    NVNdevice* getNvnDevice() const { return mNvnDevice; }

    NVNtexturePool* getTexturePool() { return &mNvnTexturePool; }

    s32 getTextureSamplerID() const { return mTextureSamplerID; }

    CriticalSection* getCriticalSection2() { return &mCriticalSection2; }

    static GraphicsNvn* instance() { return (GraphicsNvn*)Graphics::instance(); }

private:
    void defaultNvnDebugCallback_(const NvnDebugCallbackParam&);

    NVNdevice* mNvnDevice;
    void* _38;
    void* _40;
    void* _48;
    void* _50;
    NVNtexturePool mNvnTexturePool;
    void* _78;
    void* _80;
    void* _88;
    void* _90;
    void* _98;
    void* _A0;
    void* _A8;
    void* _B0;
    void* _B8;
    void* _C0;
    void* _C8;
    void* _D0;
    void* _D8;
    void* _E0;
    void* _E8;
    void* _F0;
    s32 mTextureSamplerID;
    void* _100;
    s32 _108;
    s32 _10C;
    s32 _110;
    s32 _114;
    CriticalSection mCriticalSection1;
    CriticalSection mCriticalSection2;
    CriticalSection mCriticalSection3;
    void* _1D8;
    void* _1E0;
    void* _1E8;
    void* _1F0;
    void* _1F8;
    bool _200;
    bool _201;
    bool _202;
};
static_assert(sizeof(GraphicsNvn) == 0x208);

}  // namespace sead
