#ifndef SEAD_PRIMITIVE_RENDERER_CAFE_H_
#define SEAD_PRIMITIVE_RENDERER_CAFE_H_

#include <cafe.h>

#include "sead/seadMatrix.h"
#include "sead/seadPrimitiveRenderer.h"
//#include "sead/seadPrimitiveRendererUtil.h"
#include "sead/seadSafeString.h"
#include "sead/seadVector.h"

namespace sead {

class Heap;
class Camera;
class Projection;
struct Color4f;
class Texture;

namespace PrimitiveRendererUtil {

struct Vertex;

}

class PrimitiveRendererCafe : public PrimitiveRendererBase
{
public:
    PrimitiveRendererCafe(Heap* heap);

    virtual void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32);
    virtual void prepareImpl(Heap* heap, const SafeString& path);
    virtual void setCameraImpl(const Camera& camera);
    virtual void setProjectionImpl(const Projection& projection);
    virtual void beginImpl();
    virtual void endImpl();
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR);
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture, const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src, const Vector2f& uv_size);
    virtual void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR);
    virtual void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1);
    virtual void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south);
    virtual void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south);
    virtual void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge);
    virtual void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge);
    virtual void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge);
    virtual void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge);
    virtual void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm);
    virtual void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm);

    virtual ~PrimitiveRendererCafe() { }

    void drawTriangles_(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1, PrimitiveRendererUtil::Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num, const GX2Texture* tex);
    void drawLines_(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1, PrimitiveRendererUtil::Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num);

    Matrix34f mCameraMtx;
    Matrix44f mProjectionMtx;
    GX2VertexShader* mVertexShader;
    GX2PixelShader* mPixelShader;
    u32 mParamWVPOffset;
    u32 mParamUserOffset;
    u32 mParamRateOffset;
    u32 mParamColor0Offset;
    u32 mParamColor1Offset;
    u32 mParamTexLocation;
    u32 mAttrVertexLocation;
    u32 mAttrTexCoord0Location;
    u32 mAttrColorRateLocation;
    GX2AttribStream mAttrVertex;
    GX2AttribStream mAttrTexCoord0;
    GX2AttribStream mAttrColorRate;
    void* mFsBuffer;
    GX2FetchShader mFetchShader;
    GX2Sampler mSampler;

    // Quad, Box
    PrimitiveRendererUtil::Vertex* mQuadVertex;
    u16*                           mQuadIndex;
    u16*                           mBoxIndex;

    // Line
    PrimitiveRendererUtil::Vertex* mLineVertex;
    u16*                           mLineIndex;

    // Cube
    PrimitiveRendererUtil::Vertex* mCubeVertex;
    u16*                           mCubeIndex;

    // WireCube
    PrimitiveRendererUtil::Vertex* mWireCubeVertex;
    u16*                           mWireCubeIndex;

    // Sphere4x8
    PrimitiveRendererUtil::Vertex* mSphereVertexS;
    u16*                           mSphereIndexS;

    // Sphere8x16
    PrimitiveRendererUtil::Vertex* mSphereVertexL;
    u16*                           mSphereIndexL;

    // Disk16, Disk32, Circle16, Circle32
    PrimitiveRendererUtil::Vertex* mDiskVertexS;
    u16*                           mDiskIndexS;
    PrimitiveRendererUtil::Vertex* mDiskVertexL;
    u16*                           mDiskIndexL;
    u16*                           mCircleIndexS;
    u16*                           mCircleIndexL;

    // Cylinder16
    PrimitiveRendererUtil::Vertex* mCylinderVertexS;
    u16*                           mCylinderIndexS;

    // Cylinder32
    PrimitiveRendererUtil::Vertex* mCylinderVertexL;
    u16*                           mCylinderIndexL;
};

} // namespace sead

#endif // SEAD_PRIMITIVE_RENDERER_H_
