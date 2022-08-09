#pragma once

#include <gfx/seadColor.h>
#include <math/seadBoundBox.h>

namespace sead
{
class Viewport;
class Camera;
class Projection;
class DrawContext;
class FontBase;
class DebugFontMgrNvn;

class TextWriter
{
public:
    TextWriter(DrawContext*);
    TextWriter(DrawContext*, const Viewport*);
    virtual ~TextWriter();

    DebugFontMgrNvn* getDefaultFont();
    static void setDefaultFont(FontBase*);
    static void setupGraphics(DrawContext*);
    void getCursorFromTopLeft(Vector2f*);
    void setCursorFromTopLeft(const Vector2f& pos);
    void setScaleFromFontSize(const Vector2f&);
    void setScaleFromFontHeight(float scale);
    void setProjectionAndCamera(const Projection*, const Camera*);
    void setLineSpaceFromLineHeight(float);
    void setFormatBuffer(char16_t*, int);
    void setDrawContext(DrawContext*);
    void beginDraw();
    void endDraw();
    void printf(const char16_t* format, ...);
    void vprintfImpl_(const char16_t*, std::va_list, bool, BoundBox2f*);
    void printfWithCalcRect(BoundBox2f*, const char16_t*, ...);
    void printf(const char* format, ...);
    void vprintfImpl_(const char*, std::va_list, bool, BoundBox2f*);
    void printfWithCalcRect(BoundBox2f*, const char*, ...);
    void calcFormatStringRect(BoundBox2f*, const char16_t*, ...);
    void calcFormatStringRect(BoundBox2f*, const char*, ...);
    void printImpl_(const char16_t*, int, bool, BoundBox2f*, const Projection*, const Camera*);
    void printImpl_(const char16_t*, int, bool, BoundBox2f*);
    void printImpl_(const char*, int, bool, BoundBox2f*);

private:
    Viewport* mViewport;
    Projection* mProjection;
    Camera* mCamera;
    int _20;
    int _24;
    int _28;
    int _2c;
    Vector2f mScale;
    Color4f mColor;
    int _48;
    float mLineSpace;
    BoundBox2f mBoundBox2;
    float _58;
    int _5c;
    char16_t* mFormatBuffer;
    int mFormatBufferSize;
    int _6c;
    DrawContext* mDrawContext;
};
}  // namespace sead
