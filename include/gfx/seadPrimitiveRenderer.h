#ifndef SEAD_PRIMITIVE_RENDERER_H_
#define SEAD_PRIMITIVE_RENDERER_H_

#include <gfx/seadColor.h>
#include <heap/seadDisposer.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace sead
{
class Heap;
class Camera;
class Projection;
class Texture;
class DrawContext;
class PrimitiveDrawMgrNvn;

class PrimitiveDrawer
{
public:
    class QuadArg
    {
    public:
        inline QuadArg(Vector3f pos, Vector2f size, Color4f color = Color4f(255, 255, 255, 1),
                       Color4f color2 = Color4f(255, 255, 255, 1), bool isHorizontal = false)
        {
            mCenter = pos;
            mSize = size;
            mColor0 = color;
            mColor1 = color;
            mHorizontal = isHorizontal;
        };
        QuadArg()
            : mCenter(Vector3f::zero), mSize(Vector2f::zero), mColor0(Color4f::cWhite),
              mColor1(Color4f::cWhite), mHorizontal(false)
        {
        }

        void setCornerAndSize(const Vector3f&, const Vector2f&);
        void setBoundBox(const BoundBox2f&, float);
        void setColor(const Color4f&, const Color4f&);
        void setColorHorizontal(const Color4f&, const Color4f&);

        Vector3f mCenter;
        Vector2f mSize;
        Color4f mColor0;
        Color4f mColor1;
        bool mHorizontal;
    };

    class CubeArg
    {
    public:
        inline CubeArg(const BoundBox3f& box, const Color4f& color)
        {
            mCenter = box.getCenter();
            mSize = {box.getSizeX(), box.getSizeY(), box.getSizeZ()};
            mColor0 = color;
            mColor1 = color;
        };
        inline CubeArg(const Vector3f& pos, const Vector3f& size, const Color4f& color)
        {
            mCenter = pos;
            mSize = size;
            mColor0 = color;
            mColor1 = color;
        };
        void setCornerAndSize(const Vector3f&, const Vector3f&);
        void setBoundBox(const BoundBox3f&);

    private:
        Vector3f mCenter;
        Vector3f mSize;
        Color4f mColor0;
        Color4f mColor1;
    };

    class UVArg
    {
    private:
        Vector2f vec1;
        Vector2f vec2;
    };

    PrimitiveDrawer(DrawContext* context);
    virtual ~PrimitiveDrawer();
    void prepareMgrFromBinary(Heap*, const void*, u32);
    void createDrawMgrInstance_(Heap*);
    void prepareMgr(Heap*, const SafeString&);
    void prepareMgrFromBinary(Heap*, const void*, u32, u32, bool);
    void prepareMgr(Heap*, const SafeString&, u32, bool);
    void setCamera(const Camera*);
    void setCameraViewMatrix(const Matrix34f*);
    void setProjection(const Projection*);
    void setProjectionMatrix(const Matrix44f*);
    void setModelMatrix(const Matrix34f*);
    void setDrawContext(DrawContext*);
    void begin(void);
    PrimitiveDrawMgrNvn* getDrawMgr_(void);
    void end(void);
    void drawQuad(const Color4f&, const Color4f&);
    void drawQuad(const Texture&, const Color4f&, const Color4f&, const Vector2f&, const Vector2f&);
    void drawBox(const Color4f&, const Color4f&);
    void drawCube(const Color4f&, const Color4f&);
    void drawWireCube(const Color4f&, const Color4f&);
    void drawLine(const Color4f&, const Color4f&);
    void drawSphere4x8(const Color4f&, const Color4f&);
    void drawSphere8x16(const Color4f&, const Color4f&);
    void drawDisk16(const Color4f&, const Color4f&);
    void drawDisk32(const Color4f&, const Color4f&);
    void drawCircle16(const Color4f&);
    void drawCircle32(const Color4f&);
    void drawCylinder16(const Color4f&, const Color4f&);
    void drawCylinder32(const Color4f&, const Color4f&);
    void drawQuad(const QuadArg&);
    void drawQuad(const Texture&, const QuadArg&, const UVArg&);
    void drawBox(const QuadArg&);
    void drawCube(const CubeArg&);
    void drawWireCube(const CubeArg&);
    void drawLine(const Vector3f&, const Vector3f&, const Color4f&, const Color4f&);
    void drawLine(const Vector3f&, const Vector3f&, const Color4f&);
    void drawSphere4x8(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawSphere4x8(const Vector3f&, float, const Color4f&);
    void drawSphere8x16(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawSphere8x16(const Vector3f&, float, const Color4f&);
    void drawDisk16(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawDisk16(const Vector3f&, float, const Color4f&);
    void drawDisk32(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawDisk32(const Vector3f&, float, const Color4f&);
    void drawCircle16(const Vector3f&, float, const Color4f&);
    void drawCircle32(const Vector3f&, float, const Color4f&);
    void drawCylinder16(const Vector3f&, float, float, const Color4f&, const Color4f&);
    void drawCylinder16(const Vector3f&, float, float, const Color4f&);
    void drawCylinder32(const Vector3f&, float, float, const Color4f&, const Color4f&);
    void drawCylinder32(const Vector3f&, float, float, const Color4f&);
    void drawAxis(const Vector3f&, float);

private:
    Matrix34f* mModel;
    Matrix34f* mCamera;
    Matrix44f* mProjection;
    DrawContext* mDrawContext;
};

class PrimitiveRendererBase
{
public:
    virtual void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size) = 0;
    virtual void prepareImpl(Heap* heap, const SafeString& path) = 0;
    virtual void setCameraImpl(const Camera& camera) = 0;
    virtual void setProjectionImpl(const Projection& projection) = 0;
    virtual void beginImpl() = 0;
    virtual void endImpl() = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                              const Color4f& colorR) = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture,
                              const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src,
                              const Vector2f& uv_size) = 0;
    virtual void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL,
                             const Color4f& colorR) = 0;
    virtual void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0,
                                  const Color4f& c1) = 0;
    virtual void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north,
                                   const Color4f& south) = 0;
    virtual void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north,
                                    const Color4f& south) = 0;
    virtual void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center,
                                const Color4f& edge) = 0;
    virtual void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center,
                                const Color4f& edge) = 0;
    virtual void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top,
                                    const Color4f& btm) = 0;
    virtual void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top,
                                    const Color4f& btm) = 0;
};

class PrimitiveRenderer
{
    SEAD_SINGLETON_DISPOSER(PrimitiveRenderer)
public:
    PrimitiveRenderer();
    virtual ~PrimitiveRenderer() {}

    void prepareFromBinary(Heap* heap, const void* bin_data, u32 bin_size);
    void prepare(Heap* heap, const SafeString& path);
    void setCamera(const Camera& camera);
    void setProjection(const Projection& projection);
    void setModelMatrix(const Matrix34f& model_mtx);
    void begin();
    void end();

    void drawQuad(const Color4f&, const Color4f&);
    void drawQuad(const Texture&, const Color4f&, const Color4f&, const Vector2f&, const Vector2f&);
    void drawBox(const Color4f&, const Color4f&);
    void drawCube(const Color4f&, const Color4f&);
    void drawWireCube(const Color4f&, const Color4f&);
    void drawLine(const Color4f&, const Color4f&);
    void drawSphere4x8(const Color4f&, const Color4f&);
    void drawSphere8x16(const Color4f&, const Color4f&);
    void drawDisk16(const Color4f&, const Color4f&);
    void drawDisk32(const Color4f&, const Color4f&);
    void drawCircle16(const Color4f&);
    void drawCircle32(const Color4f&);
    void drawCylinder16(const Color4f&, const Color4f&);
    void drawCylinder32(const Color4f&, const Color4f&);

    void drawQuad(const PrimitiveDrawer::QuadArg&);
    void drawQuad(const Texture&, const PrimitiveDrawer::QuadArg&, const PrimitiveDrawer::UVArg&);
    void drawBox(const PrimitiveDrawer::QuadArg&);
    void drawCube(const PrimitiveDrawer::CubeArg&);
    void drawWireCube(const PrimitiveDrawer::CubeArg&);
    void drawLine(const Vector3f&, const Vector3f&, const Color4f&, const Color4f&);
    void drawLine(const Vector3f&, const Vector3f&, const Color4f&);
    void drawSphere4x8(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawSphere4x8(const Vector3f&, float, const Color4f&);
    void drawSphere8x16(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawSphere8x16(const Vector3f&, float, const Color4f&);
    void drawDisk16(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawDisk16(const Vector3f&, float, const Color4f&);
    void drawDisk32(const Vector3f&, float, const Color4f&, const Color4f&);
    void drawDisk32(const Vector3f&, float, const Color4f&);
    void drawCircle16(const Vector3f&, float, const Color4f&);
    void drawCircle32(const Vector3f&, float, const Color4f&);
    void drawCylinder16(const Vector3f&, float, float, const Color4f&, const Color4f&);
    void drawCylinder16(const Vector3f&, float, float, const Color4f&);
    void drawCylinder32(const Vector3f&, float, float, const Color4f&, const Color4f&);
    void drawCylinder32(const Vector3f&, float, float, const Color4f&);

    void drawAxis(const Vector3f&, float);

private:
    void doPrepare_(Heap* heap);

    PrimitiveDrawer mDrawer;
    Matrix34f mCameraMatrix;
    Matrix44f mProjection;
    Matrix34f mModelMatrix;
};

}  // namespace sead

#endif  // SEAD_PRIMITIVE_RENDERER_H_
