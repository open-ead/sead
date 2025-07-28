#include <gfx/seadGraphicsContext.h>

namespace sead
{

GraphicsContext::GraphicsContext()
#ifdef cafe

    :

      mDepthTestEnable(true), mDepthWriteEnable(true), mDepthFunc(Graphics::cDepthFunc_LessEqual),
      mCullingMode(Graphics::cCullingMode_Back)

      ,
      mBlendEnable(true), mBlendFactorSrcRGB(Graphics::cBlendFactor_SrcAlpha),
      mBlendFactorSrcA(Graphics::cBlendFactor_SrcAlpha),
      mBlendFactorDstRGB(Graphics::cBlendFactor_InvSrcAlpha),
      mBlendFactorDstA(Graphics::cBlendFactor_InvSrcAlpha),
      mBlendEquationRGB(Graphics::cBlendEquation_Add),
      mBlendEquationA(Graphics::cBlendEquation_Add), mBlendConstantColor(1.0f, 1.0f, 1.0f, 1.0f)

      ,
      mAlphaTestEnable(false), mAlphaTestFunc(Graphics::cAlphaFunc_Greater), mAlphaTestRef(0.0f)

      ,
      mColorMaskR(true), mColorMaskG(true), mColorMaskB(true), mColorMaskA(true)

      ,
      mStencilTestEnable(false), mStencilTestFunc(Graphics::cStencilFunc_Never), mStencilTestRef(0),
      mStencilTestMask(0xFFFFFFFF), mStencilOpFail(Graphics::cStencilOp_Keep),
      mStencilOpZFail(Graphics::cStencilOp_Keep), mStencilOpZPass(Graphics::cStencilOp_Keep)

      ,
      mPolygonModeFront(Graphics::cPolygonMode_Fill), mPolygonModeBack(Graphics::cPolygonMode_Fill),
      mPolygonOffsetFrontEnable(false), mPolygonOffsetBackEnable(false),
      mPolygonOffsetPointLineEnable(false)
#endif  // cafe
{
}

void GraphicsContext::apply() const
{
#ifdef cafe
    GX2SetDepthStencilControl(
        static_cast<GX2Boolean>(mDepthTestEnable), static_cast<GX2Boolean>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc), static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail));

    GX2SetStencilMask(mStencilTestMask, mStencilTestRef,
                      mStencilTestRef,  // Bug: ref in place of write mask
                      mStencilTestMask, mStencilTestRef, mStencilTestRef);  // ^^^

    {
        static const GX2Boolean cCullSettings[4][2] = {{GX2_ENABLE, GX2_DISABLE},
                                                       {GX2_DISABLE, GX2_ENABLE},
                                                       {GX2_DISABLE, GX2_DISABLE},
                                                       {GX2_ENABLE, GX2_ENABLE}};
        const GX2Boolean* const& settings = cCullSettings[mCullingMode];

        GX2SetPolygonControl(
            GX2_FRONT_FACE_CCW, settings[0], settings[1],
            static_cast<GX2Boolean>(mPolygonModeFront != Graphics::cPolygonMode_Fill ||
                                    mPolygonModeBack != Graphics::cPolygonMode_Fill),
            static_cast<GX2PolygonMode>(mPolygonModeFront),
            static_cast<GX2PolygonMode>(mPolygonModeBack),
            static_cast<GX2Boolean>(mPolygonOffsetFrontEnable),
            static_cast<GX2Boolean>(mPolygonOffsetBackEnable),
            static_cast<GX2Boolean>(mPolygonOffsetPointLineEnable));
    }

    GX2SetColorControl(
        GX2_LOGIC_OP_NONE, mBlendEnable ? 0xFF : 0, GX2_DISABLE,
        static_cast<GX2Boolean>(mColorMaskR || mColorMaskG || mColorMaskB || mColorMaskA));

    GX2SetBlendControl(GX2_RENDER_TARGET_0, static_cast<GX2BlendFunction>(mBlendFactorSrcRGB),
                       static_cast<GX2BlendFunction>(mBlendFactorDstRGB),
                       static_cast<GX2BlendCombine>(mBlendEquationRGB), GX2_ENABLE,
                       static_cast<GX2BlendFunction>(mBlendFactorSrcA),
                       static_cast<GX2BlendFunction>(mBlendFactorDstA),
                       static_cast<GX2BlendCombine>(mBlendEquationA));

    GX2SetBlendConstantColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b,
                             mBlendConstantColor.a);

    GX2SetAlphaTest(static_cast<GX2Boolean>(mAlphaTestEnable),
                    static_cast<GX2CompareFunction>(mAlphaTestFunc), mAlphaTestRef);

    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(
            mColorMaskR * GX2_CHANNEL_MASK_R + mColorMaskG * GX2_CHANNEL_MASK_G +
            mColorMaskB * GX2_CHANNEL_MASK_B + mColorMaskA * GX2_CHANNEL_MASK_A),
        GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE);

    GX2SetAlphaToMask(GX2_DISABLE, GX2_ALPHA_TO_MASK_0);
#endif  // cafe
}

void GraphicsContext::applyAlphaTest() const
{
#ifdef cafe
    GX2SetAlphaTest(static_cast<GX2Boolean>(mAlphaTestEnable),
                    static_cast<GX2CompareFunction>(mAlphaTestFunc), mAlphaTestRef);
#endif  // cafe
}

void GraphicsContext::applyDepthAndStencilTest() const
{
#ifdef cafe
    GX2SetDepthStencilControl(
        static_cast<GX2Boolean>(mDepthTestEnable), static_cast<GX2Boolean>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc), static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail));

    GX2SetStencilMask(mStencilTestMask, mStencilTestRef,
                      mStencilTestRef,  // Bug: ref in place of write mask
                      mStencilTestMask, mStencilTestRef, mStencilTestRef);  // ^^^
#endif                                                                      // cafe
}

void GraphicsContext::applyColorMask() const
{
#ifdef cafe
    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(
            mColorMaskR * GX2_CHANNEL_MASK_R + mColorMaskG * GX2_CHANNEL_MASK_G +
            mColorMaskB * GX2_CHANNEL_MASK_B + mColorMaskA * GX2_CHANNEL_MASK_A),
        GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE,
        GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE, GX2_CHANNEL_MASK_NONE);
#endif  // cafe
}

void GraphicsContext::applyBlendAndFastZ() const
{
#ifdef cafe
    GX2SetColorControl(
        GX2_LOGIC_OP_NONE, mBlendEnable ? 0xFF : 0, GX2_DISABLE,
        static_cast<GX2Boolean>(mColorMaskR || mColorMaskG || mColorMaskB || mColorMaskA));

    GX2SetBlendControl(GX2_RENDER_TARGET_0, static_cast<GX2BlendFunction>(mBlendFactorSrcRGB),
                       static_cast<GX2BlendFunction>(mBlendFactorDstRGB),
                       static_cast<GX2BlendCombine>(mBlendEquationRGB), GX2_ENABLE,
                       static_cast<GX2BlendFunction>(mBlendFactorSrcA),
                       static_cast<GX2BlendFunction>(mBlendFactorDstA),
                       static_cast<GX2BlendCombine>(mBlendEquationA));
#endif  // cafe
}

void GraphicsContext::applyBlendConstantColor() const
{
#ifdef cafe
    GX2SetBlendConstantColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b,
                             mBlendConstantColor.a);
#endif  // cafe
}

void GraphicsContext::applyCullingAndPolygonModeAndPolygonOffset() const
{
#ifdef cafe
    static const GX2Boolean cCullSettings[4][2] = {{GX2_ENABLE, GX2_DISABLE},
                                                   {GX2_DISABLE, GX2_ENABLE},
                                                   {GX2_DISABLE, GX2_DISABLE},
                                                   {GX2_ENABLE, GX2_ENABLE}};
    const GX2Boolean* const& settings = cCullSettings[mCullingMode];

    GX2SetPolygonControl(GX2_FRONT_FACE_CCW, settings[0], settings[1],
                         static_cast<GX2Boolean>(mPolygonModeFront != Graphics::cPolygonMode_Fill ||
                                                 mPolygonModeBack != Graphics::cPolygonMode_Fill),
                         static_cast<GX2PolygonMode>(mPolygonModeFront),
                         static_cast<GX2PolygonMode>(mPolygonModeBack),
                         static_cast<GX2Boolean>(mPolygonOffsetFrontEnable),
                         static_cast<GX2Boolean>(mPolygonOffsetBackEnable),
                         static_cast<GX2Boolean>(mPolygonOffsetPointLineEnable));
#endif  // cafe
}

}  // namespace sead
