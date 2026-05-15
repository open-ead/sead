#pragma once

#include <gfx/seadColor.h>
#include <gfx/seadDrawLockContext.h>
#include <heap/seadDisposer.h>

namespace sead
{
class Graphics : public IDisposer
{
protected:
    using UnknownCallback = void (*)(int);
    static Graphics* sInstance;

public:
    enum DevicePosture
    {
        cDevicePosture_Same = 0,
        cDevicePosture_RotateRight = 1,
        cDevicePosture_RotateLeft = 2,
        cDevicePosture_RotateHalfAround = 3,
        cDevicePosture_FlipX = 4,
        cDevicePosture_FlipY = 5,
        cDevicePosture_FlipXY = 3,
        cDevicePosture_Invalid = 4,
    };

    enum class AlphaFunc;
    enum class DepthFunc;
    enum class StencilFunc;
    enum class StencilOp;
    enum class PolygonMode;
    enum class BlendEquation;
    enum class BlendFactor;
    enum class CullingMode;

    virtual ~Graphics();

    void lockDrawContext();
    void unlockDrawContext();
    void initHostIO();
    virtual void initializeDrawLockContext(Heap*);
    virtual void initializeImpl(Heap*) = 0;

    virtual void setViewportImpl(f32, f32, f32, f32) = 0;
    virtual void setScissorImpl(f32, f32, f32, f32) = 0;
    virtual void setDepthTestEnableImpl(bool) = 0;
    virtual void setDepthWriteEnableImpl(bool) = 0;
    virtual void setDepthFuncImpl(Graphics::DepthFunc) = 0;
    virtual void setVBlankWaitIntervalImpl(u32) = 0;
    virtual void setCullingModeImpl(Graphics::CullingMode) = 0;
    virtual void setBlendEnableImpl(bool) = 0;
    virtual void setBlendEnableMRTImpl(u32, bool) = 0;
    virtual void setBlendFactorImpl(Graphics::BlendFactor, Graphics::BlendFactor,
                                    Graphics::BlendFactor, Graphics::BlendFactor) = 0;
    virtual void setBlendFactorMRTImpl(u32, Graphics::BlendFactor, Graphics::BlendFactor,
                                       Graphics::BlendFactor, Graphics::BlendFactor) = 0;
    virtual void setBlendEquationImpl(Graphics::BlendEquation, Graphics::BlendEquation) = 0;
    virtual void setBlendEquationMRTImpl(u32, Graphics::BlendEquation, Graphics::BlendEquation) = 0;
    virtual void setBlendConstantColorImpl(sead::Color4f const&) = 0;
    virtual void waitForVBlankImpl() = 0;
    virtual void setColorMaskImpl(bool, bool, bool, bool) = 0;
    virtual void setColorMaskMRTImpl(u32, bool, bool, bool, bool) = 0;
    virtual void setAlphaTestEnableImpl(bool) = 0;
    virtual void setAlphaTestFuncImpl(Graphics::AlphaFunc, f32) = 0;
    virtual void setStencilTestEnableImpl(bool) = 0;
    virtual void setStencilTestFuncImpl(Graphics::StencilFunc, s32, u32) = 0;
    virtual void setStencilTestOpImpl(Graphics::StencilOp, Graphics::StencilOp,
                                      Graphics::StencilOp) = 0;
    virtual void setPolygonModeImpl(Graphics::PolygonMode, Graphics::PolygonMode) = 0;
    virtual void setPolygonOffsetEnableImpl(bool, bool, bool) = 0;

    static Graphics* instance() { return sInstance; }
    static DevicePosture sDefaultDevicePosture;
    static f32 sDefaultDeviceZScale;
    static f32 sDefaultDeviceZOffset;

private:
    UnknownCallback _20;
    DrawLockContext* mDrawLockContext;
};

}  // namespace sead
