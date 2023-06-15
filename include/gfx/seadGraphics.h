#ifndef SEAD_GRAPHICS_H_
#define SEAD_GRAPHICS_H_

#include <heap/seadDisposer.h>
#include <thread/seadCriticalSection.h>

#ifdef cafe
#include <cafe/gx2.h>
#endif  // cafe

namespace sead
{

class Color4f;
class Thread;

class Graphics : public sead::IDisposer
{
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
        cDevicePosture_Invalid = 4
    };

public:
    static const u32 cRenderTarget_Num = 8;

public:
    static DevicePosture getDefaultDevicePosture() { return sDefaultDevicePosture; }
    static f32 getDefaultDeviceZScale() { return sDefaultDeviceZScale; }
    static f32 getDefaultDeviceZOffset() { return sDefaultDeviceZOffset; }

public:
    // The value for each enumerator in the following enums has been confirmed to be
    // platform-specific when comparing sead between several platforms (3DS, Wii U, Switch)

    enum DepthFunc
    {
#ifdef cafe
        cDepthFunc_Never = GX2_COMPARE_NEVER,
        cDepthFunc_Less = GX2_COMPARE_LESS,
        cDepthFunc_Equal = GX2_COMPARE_EQUAL,
        cDepthFunc_LessEqual = GX2_COMPARE_LEQUAL,
        cDepthFunc_Greater = GX2_COMPARE_GREATER,
        cDepthFunc_NotEqual = GX2_COMPARE_NOTEQUAL,
        cDepthFunc_GreaterEqual = GX2_COMPARE_GEQUAL,
        cDepthFunc_Always = GX2_COMPARE_ALWAYS
#endif  // cafe
    };

    enum CullingMode
    {
#ifdef cafe
        cCullingMode_Front = 0,
        cCullingMode_Back = 1,
        cCullingMode_None = 2,
        cCullingMode_All = 3
#endif  // cafe
    };

    enum BlendFactor
    {
#ifdef cafe
        cBlendFactor_Zero = GX2_BLEND_ZERO,
        cBlendFactor_One = GX2_BLEND_ONE,
        cBlendFactor_SrcColor = GX2_BLEND_SRC_COLOR,
        cBlendFactor_InvSrcColor = GX2_BLEND_ONE_MINUS_SRC_COLOR,
        cBlendFactor_SrcAlpha = GX2_BLEND_SRC_ALPHA,
        cBlendFactor_InvSrcAlpha = GX2_BLEND_ONE_MINUS_SRC_ALPHA,
        cBlendFactor_DstAlpha = GX2_BLEND_DST_ALPHA,
        cBlendFactor_InvDstAlpha = GX2_BLEND_ONE_MINUS_DST_ALPHA,
        cBlendFactor_DstColor = GX2_BLEND_DST_COLOR,
        cBlendFactor_InvDstColor = GX2_BLEND_ONE_MINUS_DST_COLOR,
        cBlendFactor_SrcAlphaSaturate = GX2_BLEND_SRC_ALPHA_SATURATE,
        cBlendFactor_ConstantColor = GX2_BLEND_CONSTANT_COLOR,
        cBlendFactor_InvConstantColor = GX2_BLEND_ONE_MINUS_CONSTANT_COLOR,
        cBlendFactor_ConstantAlpha = GX2_BLEND_CONSTANT_ALPHA,
        cBlendFactor_InvConstantAlpha = GX2_BLEND_ONE_MINUS_CONSTANT_ALPHA
#endif  // cafe
    };

    enum BlendEquation
    {
#ifdef cafe
        cBlendEquation_Add = GX2_BLEND_COMBINE_ADD,
        cBlendEquation_Sub = GX2_BLEND_COMBINE_SRC_MINUS_DST,
        cBlendEquation_Min = GX2_BLEND_COMBINE_MIN,
        cBlendEquation_Max = GX2_BLEND_COMBINE_MAX,
        cBlendEquation_ReverseSub = GX2_BLEND_COMBINE_DST_MINUS_SRC
#endif  // cafe
    };

    enum AlphaFunc
    {
#ifdef cafe
        cAlphaFunc_Never = GX2_COMPARE_NEVER,
        cAlphaFunc_Less = GX2_COMPARE_LESS,
        cAlphaFunc_Equal = GX2_COMPARE_EQUAL,
        cAlphaFunc_LessEqual = GX2_COMPARE_LEQUAL,
        cAlphaFunc_Greater = GX2_COMPARE_GREATER,
        cAlphaFunc_NotEqual = GX2_COMPARE_NOTEQUAL,
        cAlphaFunc_GreaterEqual = GX2_COMPARE_GEQUAL,
        cAlphaFunc_Always = GX2_COMPARE_ALWAYS
#endif  // cafe
    };

    enum StencilFunc
    {
#ifdef cafe
        cStencilFunc_Never = GX2_COMPARE_NEVER,
        cStencilFunc_Less = GX2_COMPARE_LESS,
        cStencilFunc_Equal = GX2_COMPARE_EQUAL,
        cStencilFunc_LessEqual = GX2_COMPARE_LEQUAL,
        cStencilFunc_Greater = GX2_COMPARE_GREATER,
        cStencilFunc_NotEqual = GX2_COMPARE_NOTEQUAL,
        cStencilFunc_GreaterEqual = GX2_COMPARE_GEQUAL,
        cStencilFunc_Always = GX2_COMPARE_ALWAYS
#endif  // cafe
    };

    enum StencilOp
    {
#ifdef cafe
        cStencilOp_Keep = GX2_STENCIL_KEEP,
        cStencilOp_Zero = GX2_STENCIL_ZERO,
        cStencilOp_Replace = GX2_STENCIL_REPLACE,
        cStencilOp_Increment = GX2_STENCIL_INCR,
        cStencilOp_Decrement = GX2_STENCIL_DECR,
        cStencilOp_Invert = GX2_STENCIL_INVERT,
        cStencilOp_IncrementWrap = GX2_STENCIL_INCR_WRAP,
        cStencilOp_DecrementWrap = GX2_STENCIL_DECR_WRAP
#endif  // cafe
    };

    enum PolygonMode
    {
#ifdef cafe
        cPolygonMode_Point = GX2_POLYGON_MODE_POINT,
        cPolygonMode_Line = GX2_POLYGON_MODE_LINE,
        cPolygonMode_Fill = GX2_POLYGON_MODE_TRIANGLE
#endif  // cafe
    };

public:
    static Graphics* instance() { return sInstance; }
    static void setInstance(Graphics* impl) { sInstance = impl; }

    Graphics();
    ~Graphics() override;

private:
    Graphics(const Graphics&);
    const Graphics& operator=(const Graphics&);

protected:
    virtual void initializeImpl();
    virtual void setViewportImpl(f32 x, f32 y, f32 w, f32 h);
    virtual void setScissorImpl(f32 x, f32 y, f32 w, f32 h);
    virtual void setDepthTestEnableImpl(bool enable);
    virtual void setDepthWriteEnableImpl(bool enable);
    virtual void setDepthFuncImpl(DepthFunc func);
    virtual bool setVBlankWaitIntervalImpl(u32 interval);
    virtual void setCullingModeImpl(CullingMode mode);
    virtual void setBlendEnableImpl(bool enable);
    virtual void setBlendEnableMRTImpl(u32 target, bool enable);
    virtual void setBlendFactorImpl(BlendFactor src_factor_rgb, BlendFactor dst_factor_rgb,
                                    BlendFactor src_factor_a, BlendFactor dst_factor_a);
    virtual void setBlendFactorMRTImpl(u32 target, BlendFactor src_factor_rgb,
                                       BlendFactor dst_factor_rgb, BlendFactor src_factor_a,
                                       BlendFactor dst_factor_a);
    virtual void setBlendEquationImpl(BlendEquation equation_rgb, BlendEquation equation_a);
    virtual void setBlendEquationMRTImpl(u32 target, BlendEquation equation_rgb,
                                         BlendEquation equation_a);
    virtual void setBlendConstantColorImpl(const Color4f& color);
    virtual void lockDrawContextImpl();
    virtual void unlockDrawContextImpl();
    virtual void waitForVBlankImpl();
    virtual void setColorMaskImpl(bool r, bool g, bool b, bool a);
    virtual void setColorMaskMRTImpl(u32 target, bool r, bool g, bool b, bool a);
    virtual void setAlphaTestEnableImpl(bool enable);
    virtual void setAlphaTestFuncImpl(AlphaFunc func, f32 ref);
    virtual void setStencilTestEnableImpl(bool enable);
    virtual void setStencilTestFuncImpl(StencilFunc func, s32 ref, u32 mask);
    virtual void setStencilTestOpImpl(StencilOp fail, StencilOp zfail, StencilOp zpass);
    virtual void setPolygonModeImpl(PolygonMode front, PolygonMode back);
    virtual void setPolygonOffsetEnableImpl(bool fill_front_enable, bool fill_back_enable,
                                            bool point_line_enable);

public:
    void setViewportRealPosition(f32 x, f32 y, f32 w, f32 h) { setViewportImpl(x, y, w, h); }

    void setScissorRealPosition(f32 x, f32 y, f32 w, f32 h) { setScissorImpl(x, y, w, h); }

    void setDepthEnable(bool test_enable, bool write_enable)
    {
        setDepthTestEnableImpl(test_enable);
        setDepthWriteEnableImpl(write_enable);
    }

    void setBlendEnable(bool enable) { setBlendEnableImpl(enable); }

    void setBlendFactor(BlendFactor src_factor, BlendFactor dst_factor)
    {
        setBlendFactorImpl(src_factor, dst_factor, src_factor, dst_factor);
    }

    void setBlendFactorSeparate(BlendFactor src_factor_rgb, BlendFactor dst_factor_rgb,
                                BlendFactor src_factor_a, BlendFactor dst_factor_a)
    {
        setBlendFactorImpl(src_factor_rgb, dst_factor_rgb, src_factor_a, dst_factor_a);
    }

    void setBlendFactorMRT(u32 target, BlendFactor src_factor, BlendFactor dst_factor)
    {
        setBlendFactorMRTImpl(target, src_factor, dst_factor, src_factor, dst_factor);
    }

    void setBlendFactorSeparateMRT(u32 target, BlendFactor src_factor_rgb,
                                   BlendFactor dst_factor_rgb, BlendFactor src_factor_a,
                                   BlendFactor dst_factor_a)
    {
        setBlendFactorMRTImpl(target, src_factor_rgb, dst_factor_rgb, src_factor_a, dst_factor_a);
    }

    void setBlendEquation(BlendEquation equation) { setBlendEquationImpl(equation, equation); }

    void setBlendEquationSeparate(BlendEquation equation_rgb, BlendEquation equation_a)
    {
        setBlendEquationImpl(equation_rgb, equation_a);
    }

    void setBlendEquationMRT(u32 target, BlendEquation equation)
    {
        setBlendEquationMRTImpl(target, equation, equation);
    }

    void setBlendEquationSeparateMRT(u32 target, BlendEquation equation_rgb,
                                     BlendEquation equation_a)
    {
        setBlendEquationMRTImpl(target, equation_rgb, equation_a);
    }

    void lockDrawContext();
    void unlockDrawContext();

protected:
    Thread* mContextHolderThread;
    s32 mContextRefCounter;
    CriticalSection mContextCriticalSection;

    static Graphics* sInstance;

    static DevicePosture sDefaultDevicePosture;
    static f32 sDefaultDeviceZScale;
    static f32 sDefaultDeviceZOffset;
};
#ifdef cafe
static_assert(sizeof(Graphics) == 0x54, "sead::Graphics size mismatch");
#endif  // cafe

}  // namespace sead

#endif  // SEAD_GRAPHICS_H_
