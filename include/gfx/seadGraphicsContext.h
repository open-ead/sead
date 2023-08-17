#ifndef SEAD_GRAPHICS_CONTEXT_H_
#define SEAD_GRAPHICS_CONTEXT_H_

#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>

namespace sead
{

class GraphicsContext
{
public:
    GraphicsContext();
    virtual ~GraphicsContext() {}

    void apply() const;
    void applyAlphaTest() const;
    void applyDepthAndStencilTest() const;
    void applyColorMask() const;
    void applyBlendAndFastZ() const;
    void applyBlendConstantColor() const;
    void applyCullingAndPolygonModeAndPolygonOffset() const;

    void setDepthEnable(bool test_enable, bool write_enable)
    {
        setDepthTestEnable(test_enable);
        setDepthWriteEnable(write_enable);
    }

    void setDepthTestEnable(bool test_enable) { mDepthTestEnable = test_enable; }

    void setDepthWriteEnable(bool write_enable) { mDepthWriteEnable = write_enable; }

    void setDepthFunc(Graphics::DepthFunc func) { mDepthFunc = func; }

    void setCullingMode(Graphics::CullingMode mode) { mCullingMode = mode; }

    void setBlendEnable(bool blend) { mBlendEnable = blend; }

    void setBlendFactor(Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
    {
        setBlendFactorSrc(src_factor);
        setBlendFactorDst(dst_factor);
    }

    void setBlendFactorSeparate(Graphics::BlendFactor src_factor_rgb,
                                Graphics::BlendFactor dst_factor_rgb,
                                Graphics::BlendFactor src_factor_a,
                                Graphics::BlendFactor dst_factor_a)
    {
        setBlendFactorSrcRGB(src_factor_rgb);
        setBlendFactorDstRGB(dst_factor_rgb);
        setBlendFactorSrcAlpha(src_factor_a);
        setBlendFactorDstAlpha(dst_factor_a);
    }

    void setBlendFactorSrc(Graphics::BlendFactor factor)
    {
        setBlendFactorSrcRGB(factor);
        setBlendFactorSrcAlpha(factor);
    }

    void setBlendFactorDst(Graphics::BlendFactor factor)
    {
        setBlendFactorDstRGB(factor);
        setBlendFactorDstAlpha(factor);
    }

    void setBlendFactorSrcRGB(Graphics::BlendFactor factor) { mBlendFactorSrcRGB = factor; }

    void setBlendFactorSrcAlpha(Graphics::BlendFactor factor) { mBlendFactorSrcA = factor; }

    void setBlendFactorDstRGB(Graphics::BlendFactor factor) { mBlendFactorDstRGB = factor; }

    void setBlendFactorDstAlpha(Graphics::BlendFactor factor) { mBlendFactorDstA = factor; }

    void setBlendEquation(Graphics::BlendEquation equation)
    {
        setBlendEquationRGB(equation);
        setBlendEquationAlpha(equation);
    }

    void setBlendEquationSeparate(Graphics::BlendEquation equation_rgb,
                                  Graphics::BlendEquation equation_a)
    {
        setBlendEquationRGB(equation_rgb);
        setBlendEquationAlpha(equation_a);
    }

    void setBlendEquationRGB(Graphics::BlendEquation equation) { mBlendEquationRGB = equation; }

    void setBlendEquationAlpha(Graphics::BlendEquation equation) { mBlendEquationA = equation; }

    void setBlendConstantColor(const Color4f& color) { mBlendConstantColor = color; }

    void setAlphaTestEnable(bool enable) { mAlphaTestEnable = enable; }

    void setAlphaTestFunc(Graphics::AlphaFunc func, f32 ref)
    {
        mAlphaTestFunc = func;
        mAlphaTestRef = ref;
    }

    void setColorMask(bool r, bool g, bool b, bool a)
    {
        mColorMaskR = r;
        mColorMaskG = g;
        mColorMaskB = b;
        mColorMaskA = a;
    }

    void setStencilTestEnable(bool enable) { mStencilTestEnable = enable; }

    void setStencilTestFunc(Graphics::StencilFunc func, s32 ref, u32 mask)
    {
        mStencilTestFunc = func;
        mStencilTestRef = ref;
        mStencilTestMask = mask;
    }

    void setStencilTestOp(Graphics::StencilOp fail, Graphics::StencilOp zfail,
                          Graphics::StencilOp zpass)
    {
        mStencilOpFail = fail;
        mStencilOpZFail = zfail;
        mStencilOpZPass = zpass;
    }

    void setPolygonMode(Graphics::PolygonMode front, Graphics::PolygonMode back)
    {
#ifdef cafe
        mPolygonModeFront = front;
        mPolygonModeBack = back;
#endif  // cafe
    }

    void setPolygonOffsetEnable(bool fill_front_enable, bool fill_back_enable,
                                bool point_line_enable)
    {
#ifdef cafe
        mPolygonOffsetFrontEnable = fill_front_enable;
        mPolygonOffsetBackEnable = fill_back_enable;
        mPolygonOffsetPointLineEnable = point_line_enable;
#endif  // cafe
    }

    bool getDepthTestEnable() const { return mDepthTestEnable; }

    bool getDepthWriteEnable() const { return mDepthWriteEnable; }

    Graphics::DepthFunc getDepthFunc() const { return mDepthFunc; }

    Graphics::CullingMode getCullingMode() const { return mCullingMode; }

    bool getBlendEnable() const { return mBlendEnable; }

    Graphics::BlendFactor getBlendFactorSrcRGB() const { return mBlendFactorSrcRGB; }

    Graphics::BlendFactor getBlendFactorSrcAlpha() const { return mBlendFactorSrcA; }

    Graphics::BlendFactor getBlendFactorDstRGB() const { return mBlendFactorDstRGB; }

    Graphics::BlendFactor getBlendFactorDstAlpha() const { return mBlendFactorDstA; }

    Graphics::BlendEquation getBlendEquationRGB() const { return mBlendEquationRGB; }

    Graphics::BlendEquation getBlendEquationAlpha() const { return mBlendEquationA; }

    const Color4f& getBlendConstantColor() const { return mBlendConstantColor; }

    bool getAlphaTestEnable() const { return mAlphaTestEnable; }

    Graphics::AlphaFunc getAlphaTestFunc() const { return mAlphaTestFunc; }

    f32 getAlphaTestRef() const { return mAlphaTestRef; }

    bool getColorMaskR() const { return mColorMaskR; }

    bool getColorMaskG() const { return mColorMaskG; }

    bool getColorMaskB() const { return mColorMaskB; }

    bool getColorMaskA() const { return mColorMaskA; }

    bool getStencilTestEnable() const { return mStencilTestEnable; }

    Graphics::StencilFunc getStencilTestFunc() const { return mStencilTestFunc; }

    s32 getStencilTestRef() const { return mStencilTestRef; }

    u32 getStencilTestMask() const { return mStencilTestMask; }

    Graphics::StencilOp getStencilTestOpFail() const { return mStencilOpFail; }

    Graphics::StencilOp getStencilTestOpZFail() const { return mStencilOpZFail; }

    Graphics::StencilOp getStencilTestOpZPass() const { return mStencilOpZPass; }

    Graphics::PolygonMode getPolygonModeFront() const
    {
#ifdef cafe
        return mPolygonModeFront;
#endif  // cafe
    }

    Graphics::PolygonMode getPolygonModeBack() const
    {
#ifdef cafe
        return mPolygonModeBack;
#endif  // cafe
    }

    bool getPolygonOffsetFrontEnable() const
    {
#ifdef cafe
        return mPolygonOffsetFrontEnable;
#endif  // cafe
    }

    bool getPolygonOffsetBackEnable() const
    {
#ifdef cafe
        return mPolygonOffsetBackEnable;
#endif  // cafe
    }

    bool getPolygonOffsetPointLineEnable() const
    {
#ifdef cafe
        return mPolygonOffsetPointLineEnable;
#endif  // cafe
    }

private:
    bool mDepthTestEnable;
    bool mDepthWriteEnable;
    Graphics::DepthFunc mDepthFunc;
    Graphics::CullingMode mCullingMode;
    bool mBlendEnable;
    Graphics::BlendFactor mBlendFactorSrcRGB;
    Graphics::BlendFactor mBlendFactorSrcA;
    Graphics::BlendFactor mBlendFactorDstRGB;
    Graphics::BlendFactor mBlendFactorDstA;
    Graphics::BlendEquation mBlendEquationRGB;
    Graphics::BlendEquation mBlendEquationA;
    Color4f mBlendConstantColor;
    bool mAlphaTestEnable;
    Graphics::AlphaFunc mAlphaTestFunc;
    f32 mAlphaTestRef;
    bool mColorMaskR;
    bool mColorMaskG;
    bool mColorMaskB;
    bool mColorMaskA;
    bool mStencilTestEnable;
    Graphics::StencilFunc mStencilTestFunc;
    s32 mStencilTestRef;
    u32 mStencilTestMask;
    Graphics::StencilOp mStencilOpFail;
    Graphics::StencilOp mStencilOpZFail;
    Graphics::StencilOp mStencilOpZPass;
#ifdef cafe
    Graphics::PolygonMode mPolygonModeFront;
    Graphics::PolygonMode mPolygonModeBack;
    bool mPolygonOffsetFrontEnable;
    bool mPolygonOffsetBackEnable;
    bool mPolygonOffsetPointLineEnable;
#endif  // cafe
};
#ifdef cafe
static_assert(sizeof(GraphicsContext) == 0x74, "sead::GraphicsContext size mismatch");
#endif  // cafe

}  // namespace sead

#endif  // SEAD_GRAPHICS_CONTEXT_H_
#ifndef SEAD_GRAPHICS_CONTEXT_H_
#define SEAD_GRAPHICS_CONTEXT_H_

#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>

namespace sead
{

class GraphicsContext
{
public:
    GraphicsContext();
    virtual ~GraphicsContext() {}

    void apply() const;
    void applyAlphaTest() const;
    void applyDepthAndStencilTest() const;
    void applyColorMask() const;
    void applyBlendAndFastZ() const;
    void applyBlendConstantColor() const;
    void applyCullingAndPolygonModeAndPolygonOffset() const;

    void setDepthEnable(bool test_enable, bool write_enable)
    {
        setDepthTestEnable(test_enable);
        setDepthWriteEnable(write_enable);
    }

    void setDepthTestEnable(bool test_enable) { mDepthTestEnable = test_enable; }

    void setDepthWriteEnable(bool write_enable) { mDepthWriteEnable = write_enable; }

    void setDepthFunc(Graphics::DepthFunc func) { mDepthFunc = func; }

    void setCullingMode(Graphics::CullingMode mode) { mCullingMode = mode; }

    void setBlendEnable(bool blend) { mBlendEnable = blend; }

    void setBlendFactor(Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
    {
        setBlendFactorSrc(src_factor);
        setBlendFactorDst(dst_factor);
    }

    void setBlendFactorSeparate(Graphics::BlendFactor src_factor_rgb,
                                Graphics::BlendFactor dst_factor_rgb,
                                Graphics::BlendFactor src_factor_a,
                                Graphics::BlendFactor dst_factor_a)
    {
        setBlendFactorSrcRGB(src_factor_rgb);
        setBlendFactorDstRGB(dst_factor_rgb);
        setBlendFactorSrcAlpha(src_factor_a);
        setBlendFactorDstAlpha(dst_factor_a);
    }

    void setBlendFactorSrc(Graphics::BlendFactor factor)
    {
        setBlendFactorSrcRGB(factor);
        setBlendFactorSrcAlpha(factor);
    }

    void setBlendFactorDst(Graphics::BlendFactor factor)
    {
        setBlendFactorDstRGB(factor);
        setBlendFactorDstAlpha(factor);
    }

    void setBlendFactorSrcRGB(Graphics::BlendFactor factor) { mBlendFactorSrcRGB = factor; }

    void setBlendFactorSrcAlpha(Graphics::BlendFactor factor) { mBlendFactorSrcA = factor; }

    void setBlendFactorDstRGB(Graphics::BlendFactor factor) { mBlendFactorDstRGB = factor; }

    void setBlendFactorDstAlpha(Graphics::BlendFactor factor) { mBlendFactorDstA = factor; }

    void setBlendEquation(Graphics::BlendEquation equation)
    {
        setBlendEquationRGB(equation);
        setBlendEquationAlpha(equation);
    }

    void setBlendEquationSeparate(Graphics::BlendEquation equation_rgb,
                                  Graphics::BlendEquation equation_a)
    {
        setBlendEquationRGB(equation_rgb);
        setBlendEquationAlpha(equation_a);
    }

    void setBlendEquationRGB(Graphics::BlendEquation equation) { mBlendEquationRGB = equation; }

    void setBlendEquationAlpha(Graphics::BlendEquation equation) { mBlendEquationA = equation; }

    void setBlendConstantColor(const Color4f& color) { mBlendConstantColor = color; }

    void setAlphaTestEnable(bool enable) { mAlphaTestEnable = enable; }

    void setAlphaTestFunc(Graphics::AlphaFunc func, f32 ref)
    {
        mAlphaTestFunc = func;
        mAlphaTestRef = ref;
    }

    void setColorMask(bool r, bool g, bool b, bool a)
    {
        mColorMaskR = r;
        mColorMaskG = g;
        mColorMaskB = b;
        mColorMaskA = a;
    }

    void setStencilTestEnable(bool enable) { mStencilTestEnable = enable; }

    void setStencilTestFunc(Graphics::StencilFunc func, s32 ref, u32 mask)
    {
        mStencilTestFunc = func;
        mStencilTestRef = ref;
        mStencilTestMask = mask;
    }

    void setStencilTestOp(Graphics::StencilOp fail, Graphics::StencilOp zfail,
                          Graphics::StencilOp zpass)
    {
        mStencilOpFail = fail;
        mStencilOpZFail = zfail;
        mStencilOpZPass = zpass;
    }

    void setPolygonMode(Graphics::PolygonMode front, Graphics::PolygonMode back)
    {
#ifdef cafe
        mPolygonModeFront = front;
        mPolygonModeBack = back;
#endif  // cafe
    }

    void setPolygonOffsetEnable(bool fill_front_enable, bool fill_back_enable,
                                bool point_line_enable)
    {
#ifdef cafe
        mPolygonOffsetFrontEnable = fill_front_enable;
        mPolygonOffsetBackEnable = fill_back_enable;
        mPolygonOffsetPointLineEnable = point_line_enable;
#endif  // cafe
    }

    bool getDepthTestEnable() const { return mDepthTestEnable; }

    bool getDepthWriteEnable() const { return mDepthWriteEnable; }

    Graphics::DepthFunc getDepthFunc() const { return mDepthFunc; }

    Graphics::CullingMode getCullingMode() const { return mCullingMode; }

    bool getBlendEnable() const { return mBlendEnable; }

    Graphics::BlendFactor getBlendFactorSrcRGB() const { return mBlendFactorSrcRGB; }

    Graphics::BlendFactor getBlendFactorSrcAlpha() const { return mBlendFactorSrcA; }

    Graphics::BlendFactor getBlendFactorDstRGB() const { return mBlendFactorDstRGB; }

    Graphics::BlendFactor getBlendFactorDstAlpha() const { return mBlendFactorDstA; }

    Graphics::BlendEquation getBlendEquationRGB() const { return mBlendEquationRGB; }

    Graphics::BlendEquation getBlendEquationAlpha() const { return mBlendEquationA; }

    const Color4f& getBlendConstantColor() const { return mBlendConstantColor; }

    bool getAlphaTestEnable() const { return mAlphaTestEnable; }

    Graphics::AlphaFunc getAlphaTestFunc() const { return mAlphaTestFunc; }

    f32 getAlphaTestRef() const { return mAlphaTestRef; }

    bool getColorMaskR() const { return mColorMaskR; }

    bool getColorMaskG() const { return mColorMaskG; }

    bool getColorMaskB() const { return mColorMaskB; }

    bool getColorMaskA() const { return mColorMaskA; }

    bool getStencilTestEnable() const { return mStencilTestEnable; }

    Graphics::StencilFunc getStencilTestFunc() const { return mStencilTestFunc; }

    s32 getStencilTestRef() const { return mStencilTestRef; }

    u32 getStencilTestMask() const { return mStencilTestMask; }

    Graphics::StencilOp getStencilTestOpFail() const { return mStencilOpFail; }

    Graphics::StencilOp getStencilTestOpZFail() const { return mStencilOpZFail; }

    Graphics::StencilOp getStencilTestOpZPass() const { return mStencilOpZPass; }

    Graphics::PolygonMode getPolygonModeFront() const
    {
#ifdef cafe
        return mPolygonModeFront;
#endif  // cafe
    }

    Graphics::PolygonMode getPolygonModeBack() const
    {
#ifdef cafe
        return mPolygonModeBack;
#endif  // cafe
    }

    bool getPolygonOffsetFrontEnable() const
    {
#ifdef cafe
        return mPolygonOffsetFrontEnable;
#endif  // cafe
    }

    bool getPolygonOffsetBackEnable() const
    {
#ifdef cafe
        return mPolygonOffsetBackEnable;
#endif  // cafe
    }

    bool getPolygonOffsetPointLineEnable() const
    {
#ifdef cafe
        return mPolygonOffsetPointLineEnable;
#endif  // cafe
    }

private:
    bool mDepthTestEnable;
    bool mDepthWriteEnable;
    Graphics::DepthFunc mDepthFunc;
    Graphics::CullingMode mCullingMode;
    bool mBlendEnable;
    Graphics::BlendFactor mBlendFactorSrcRGB;
    Graphics::BlendFactor mBlendFactorSrcA;
    Graphics::BlendFactor mBlendFactorDstRGB;
    Graphics::BlendFactor mBlendFactorDstA;
    Graphics::BlendEquation mBlendEquationRGB;
    Graphics::BlendEquation mBlendEquationA;
    Color4f mBlendConstantColor;
    bool mAlphaTestEnable;
    Graphics::AlphaFunc mAlphaTestFunc;
    f32 mAlphaTestRef;
    bool mColorMaskR;
    bool mColorMaskG;
    bool mColorMaskB;
    bool mColorMaskA;
    bool mStencilTestEnable;
    Graphics::StencilFunc mStencilTestFunc;
    s32 mStencilTestRef;
    u32 mStencilTestMask;
    Graphics::StencilOp mStencilOpFail;
    Graphics::StencilOp mStencilOpZFail;
    Graphics::StencilOp mStencilOpZPass;
#ifdef cafe
    Graphics::PolygonMode mPolygonModeFront;
    Graphics::PolygonMode mPolygonModeBack;
    bool mPolygonOffsetFrontEnable;
    bool mPolygonOffsetBackEnable;
    bool mPolygonOffsetPointLineEnable;
#endif  // cafe
};
#ifdef cafe
static_assert(sizeof(GraphicsContext) == 0x74, "sead::GraphicsContext size mismatch");
#endif  // cafe

}  // namespace sead

#endif  // SEAD_GRAPHICS_CONTEXT_H_
