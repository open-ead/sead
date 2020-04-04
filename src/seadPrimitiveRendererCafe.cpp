#include <cafe.h>
#include <cafe/gfd.h>
#include <sead.h>

static const u16 idx[4] = { 0, 1, 3, 2 };

namespace sead {

PrimitiveRendererCafe::PrimitiveRendererCafe(Heap* heap)
    : mCameraMtx()
    , mProjectionMtx()
{
}

void PrimitiveRendererCafe::prepareFromBinaryImpl(
    Heap* heap, const void* bin_data, u32 bin_size
)
{
    u32 vtxHeaderSize = GFDGetVertexShaderHeaderSize(0, bin_data);
    u32 vtxProgramSize = GFDGetVertexShaderProgramSize(0, bin_data);

    mVertexShader = static_cast<GX2VertexShader*>(heap->alloc(vtxHeaderSize, 1));
    void* vtxProgram = heap->alloc(vtxProgramSize, 0x100);

    GFDGetVertexShader(mVertexShader, vtxProgram, 0, bin_data);
    GX2Invalidate(GX2_INVALIDATE_CPU_SHADER, mVertexShader->shaderPtr, mVertexShader->shaderSize);

    u32 pixHeaderSize = GFDGetPixelShaderHeaderSize(0, bin_data);
    u32 pixProgramSize = GFDGetPixelShaderProgramSize(0, bin_data);

    mPixelShader = static_cast<GX2PixelShader*>(heap->alloc(pixHeaderSize, 1));
    void* pixProgram = heap->alloc(pixProgramSize, 0x100);

    GFDGetPixelShader(mPixelShader, pixProgram, 0, bin_data);
    GX2Invalidate(GX2_INVALIDATE_CPU_SHADER, mPixelShader->shaderPtr, mPixelShader->shaderSize);

    mParamWVPOffset = GX2GetVertexUniformVarOffset(mVertexShader, "wvp");
    mParamUserOffset = GX2GetVertexUniformVarOffset(mVertexShader, "user");
    mParamColor0Offset = GX2GetVertexUniformVarOffset(mVertexShader, "color0");
    mParamColor1Offset = GX2GetVertexUniformVarOffset(mVertexShader, "color1");
    mParamRateOffset = GX2GetPixelUniformVarOffset(mPixelShader, "rate");
    mParamTexLocation = GX2GetPixelSamplerVarLocation(mPixelShader, "texture0");
    mAttrVertexLocation = GX2GetVertexAttribVarLocation(mVertexShader, "Vertex");
    mAttrTexCoord0Location = GX2GetVertexAttribVarLocation(mVertexShader, "TexCoord0");
    mAttrColorRateLocation = GX2GetVertexAttribVarLocation(mVertexShader, "ColorRate");

    GX2InitAttribStream(&mAttrVertex, mAttrVertexLocation, 0, 0, GX2_ATTRIB_FORMAT_32_32_32_FLOAT);
    GX2InitAttribStream(&mAttrTexCoord0, mAttrTexCoord0Location, 0, GX2VarTypeToCount(GX2_VAR_TYPE_MAT3), GX2_ATTRIB_FORMAT_32_32_FLOAT);
    GX2InitAttribStream(&mAttrColorRate, mAttrColorRateLocation, 0, GX2VarTypeToCount(GX2_VAR_TYPE_MAT3) + GX2VarTypeToCount(GX2_VAR_TYPE_MAT2), GX2_ATTRIB_FORMAT_32_32_32_32_FLOAT);

    void* fs_buffer = heap->alloc(GX2CalcFetchShaderSizeEx(3, GX2_FETCH_SHADER_TESSELATION_NONE, GX2_TESSELLATION_MODE_DISCRETE), 0x100);
    mFsBuffer = fs_buffer;

    GX2InitFetchShaderEx(&mFetchShader, fs_buffer, 3, &mAttrVertex, GX2_FETCH_SHADER_TESSELATION_NONE, GX2_TESSELLATION_MODE_DISCRETE);
    GX2Invalidate(GX2_INVALIDATE_CPU_SHADER, mFsBuffer, GX2CalcFetchShaderSizeEx(3, GX2_FETCH_SHADER_TESSELATION_NONE, GX2_TESSELLATION_MODE_DISCRETE));

    // Quad
    mQuadVertex = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc(4 * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mQuadIndex  = static_cast<u16*>(heap->alloc(6 * sizeof(u16), 0x20));

    PrimitiveRendererUtil::setQuadVertex(mQuadVertex, mQuadIndex);
    mQuadVertex[0].uv.x = 0.0f;
    mQuadVertex[0].uv.y = 0.0f;
    mQuadVertex[1].uv.x = 1.0f;
    mQuadVertex[1].uv.y = 0.0f;
    mQuadVertex[2].uv.x = 0.0f;
    mQuadVertex[2].uv.y = 1.0f;
    mQuadVertex[3].uv.x = 1.0f;
    mQuadVertex[3].uv.y = 1.0f;

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mQuadVertex, 4 * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mQuadIndex, 6 * sizeof(u16));

    // Box
    mBoxIndex   = static_cast<u16*>(heap->alloc(4 * sizeof(u16), 0x20));
    memcpy(mBoxIndex, idx, 4 * sizeof(u16));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,   mBoxIndex, 4 * sizeof(u16));

    // Line
    mLineVertex = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc(4 * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mLineIndex  = static_cast<u16*>(heap->alloc(6 * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setLineVertex(mLineVertex, mLineIndex);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mLineVertex, 4 * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mLineIndex, 6 * sizeof(u16));

    // Cube
    mCubeVertex = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc(8 * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mCubeIndex  = static_cast<u16*>(heap->alloc(36 * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setCubeVertex(mCubeVertex, mCubeIndex);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mCubeVertex,  8 * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mCubeIndex, 36 * sizeof(u16));

    // WireCube
    mWireCubeVertex = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc(8 * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mWireCubeIndex  = static_cast<u16*>(heap->alloc(17 * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setWireCubeVertex(mWireCubeVertex, mWireCubeIndex);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mWireCubeVertex,  8 * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mWireCubeIndex, 17 * sizeof(u16));

    // Sphere4x8
    mSphereVertexS = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((4*8 + 2) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mSphereIndexS  = static_cast<u16*>(heap->alloc((4*8 * 6) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setSphereVertex(mSphereVertexS, mSphereIndexS, 8, 4);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mSphereVertexS, (4*8 + 2) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mSphereIndexS, (4*8 * 6) * sizeof(u16));

    // Sphere8x16
    mSphereVertexL = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((8*16 + 2) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mSphereIndexL  = static_cast<u16*>(heap->alloc((8*16 * 6) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setSphereVertex(mSphereVertexL, mSphereIndexL, 16, 8);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mSphereVertexL, (8*16 + 2) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mSphereIndexL, (8*16 * 6) * sizeof(u16));

    // Disk16
    mDiskVertexS = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((16 + 1) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mDiskIndexS  = static_cast<u16*>(heap->alloc((8 * 6) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setDiskVertex(mDiskVertexS, mDiskIndexS, 16);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mDiskVertexS, (16 + 1) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mDiskIndexS,  (8 * 6) * sizeof(u16));

    // Disk32
    mDiskVertexL = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((32 + 1) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mDiskIndexL  = static_cast<u16*>(heap->alloc((16 * 6) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setDiskVertex(mDiskVertexL, mDiskIndexL, 32);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mDiskVertexL, (32 + 1) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mDiskIndexL, (16 * 6) * sizeof(u16));

    // Disk16
    mCircleIndexS = static_cast<u16*>(heap->alloc(16 * sizeof(u16), 0x20));
    for (int i = 0; i < 16; i++)
        mCircleIndexS[i] = i;
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mCircleIndexS, 16 * sizeof(u16));

    // Disk32
    mCircleIndexL = static_cast<u16*>(heap->alloc(32 * sizeof(u16), 0x20));
    for (int i = 0; i < 32; i++)
        mCircleIndexL[i] = i;
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mCircleIndexL, 32 * sizeof(u16));

    // Cylinder16
    mCylinderVertexS = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((16*2 + 2) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mCylinderIndexS  = static_cast<u16*>(heap->alloc((16 * 12) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setCylinderVertex(mCylinderVertexS, mCylinderIndexS, 16);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mCylinderVertexS, (16*2 + 2) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mCylinderIndexS,  (16 * 12) * sizeof(u16));

    // Cylinder32
    mCylinderVertexL = static_cast<PrimitiveRendererUtil::Vertex*>(heap->alloc((32*2 + 2) * sizeof(PrimitiveRendererUtil::Vertex), 0x40));
    mCylinderIndexL  = static_cast<u16*>(heap->alloc((32 * 12) * sizeof(u16), 0x20));
    PrimitiveRendererUtil::setCylinderVertex(mCylinderVertexL, mCylinderIndexL, 32);

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, mCylinderVertexL, (32*2 + 2) * sizeof(PrimitiveRendererUtil::Vertex));
    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER,  mCylinderIndexL,  (32 * 12) * sizeof(u16));

    GX2InitSampler(&mSampler, GX2_TEX_CLAMP_CLAMP, GX2_TEX_XY_FILTER_BILINEAR);
}

void PrimitiveRendererCafe::prepareImpl(
    Heap* heap, const SafeString& path
)
{
    FileDevice::LoadArg arg;
    arg.name = path;
    arg.bufferSizeAlignment = 0x20;
    arg.heap = heap;

    const void* file = sead::FileDeviceMgr::sInstance->tryLoad(arg);
    prepareFromBinaryImpl(heap, file, arg.fileSize);
}

void PrimitiveRendererCafe::setCameraImpl(const Camera& camera)
{
    ASM_MTXCopy(const_cast<f32(*)[4]>(camera.mMatrix.rows), mCameraMtx.rows);
}

void PrimitiveRendererCafe::setProjectionImpl(const Projection& projection)
{
    ASM_MTX44Copy(const_cast<f32(*)[4]>(projection.getDeviceProjectionMatrix().rows), mProjectionMtx.rows);
}

void PrimitiveRendererCafe::beginImpl()
{
    Matrix44f WVP;
    Matrix44CalcCommon<f32>::multiply(WVP, mProjectionMtx, mCameraMtx);

    GX2VertexShader* vertexShader = mVertexShader;
    GX2PixelShader* pixelShader = mPixelShader;

    GX2SetFetchShader(&mFetchShader);
    GX2SetVertexShader(vertexShader);
    GX2SetPixelShader(pixelShader);
    GX2SetVertexUniformReg(mParamWVPOffset, 0x10, &WVP);
    GX2SetVertexUniformReg(mParamUserOffset, 0x10, &Matrix44f::ident);
    GX2SetLineWidth(2.0f);
}

void PrimitiveRendererCafe::endImpl()
{
}

void PrimitiveRendererCafe::drawQuadImpl(
    const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR
)
{
    drawTriangles_(model_mtx, colorL, colorR, mQuadVertex, 4, mQuadIndex, 6, NULL);
}

void PrimitiveRendererCafe::drawQuadImpl(
    const Matrix34f& model_mtx, const Texture& texture, const Color4f& colorL,
    const Color4f& colorR, const Vector2f& uv_src, const Vector2f& uv_size
)
{
    const TextureCafeGX2* texure_cafe_gx2 = DynamicCast<const TextureCafeGX2, const Texture>(&texture);
    drawTriangles_(model_mtx, colorL, colorR, mQuadVertex, 4, mQuadIndex, 6, texure_cafe_gx2->mGX2Texture);
}

void PrimitiveRendererCafe::drawBoxImpl(
    const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR
)
{
    drawLines_(model_mtx, colorL, colorR, mQuadVertex, 4, mBoxIndex, 4);
}

void PrimitiveRendererCafe::drawCubeImpl(
    const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawTriangles_(model_mtx, c0, c1, mCubeVertex, 8, mCubeIndex, 36, NULL);
}

void PrimitiveRendererCafe::drawWireCubeImpl(
    const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawLines_(model_mtx, c0, c1, mWireCubeVertex, 8, mWireCubeIndex, 17);
}

void PrimitiveRendererCafe::drawLineImpl(
    const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1
)
{
    drawLines_(model_mtx, c0, c1, mLineVertex, 2, mLineIndex, 2);
}

void PrimitiveRendererCafe::drawSphere4x8Impl(
    const Matrix34f& model_mtx, const Color4f& north, const Color4f& south
)
{
    drawTriangles_(model_mtx, north, south, mSphereVertexS, 34, mSphereIndexS, 192, NULL);
}

void PrimitiveRendererCafe::drawSphere8x16Impl(
    const Matrix34f& model_mtx, const Color4f& north, const Color4f& south
)
{
    drawTriangles_(model_mtx, north, south, mSphereVertexL, 130, mSphereIndexL, 768, NULL);
}

void PrimitiveRendererCafe::drawDisk16Impl(
    const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge
)
{
    drawTriangles_(model_mtx, center, edge, mDiskVertexS, 17, mDiskIndexS, 48, NULL);
}

void PrimitiveRendererCafe::drawDisk32Impl(
    const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge
)
{
    drawTriangles_(model_mtx, center, edge, mDiskVertexL, 33, mDiskIndexL, 96, NULL);
}

void PrimitiveRendererCafe::drawCircle16Impl(
    const Matrix34f& model_mtx, const Color4f& edge
)
{
    drawLines_(model_mtx, edge, edge, mDiskVertexS, 17, mCircleIndexS, 16);
}

void PrimitiveRendererCafe::drawCircle32Impl(
    const Matrix34f& model_mtx, const Color4f& edge
)
{
    drawLines_(model_mtx, edge, edge, mDiskVertexL, 33, mCircleIndexL, 32);
}

void PrimitiveRendererCafe::drawCylinder16Impl(
    const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm
)
{
    drawTriangles_(model_mtx, top, btm, mCylinderVertexS, 34, mCylinderIndexS, 192, NULL);
}

void PrimitiveRendererCafe::drawCylinder32Impl(
    const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm
)
{
    drawTriangles_(model_mtx, top, btm, mCylinderVertexL, 66, mCylinderIndexL, 384, NULL);
}

void PrimitiveRendererCafe::drawTriangles_(
    const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1,
    PrimitiveRendererUtil::Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num,
    const GX2Texture* tex
)
{
    GX2SetVertexUniformReg(mParamUserOffset, 12, &model_mtx);
    GX2SetVertexUniformReg(mParamColor0Offset, 4, &c0);
    GX2SetVertexUniformReg(mParamColor1Offset, 4, &c1);

    if (tex != NULL)
    {
        GX2SetPixelUniformReg(mParamRateOffset, 4, &Vector4f::ex);
        GX2SetPixelTexture(tex, mParamTexLocation);
        GX2SetPixelSampler(&mSampler, mParamTexLocation);
    }

    else
        GX2SetPixelUniformReg(mParamRateOffset, 4, &Vector4f::zero);

    GX2SetAttribBuffer(0, vtx_num * 0x24, 0x24, vtx);
    GX2DrawIndexedEx(GX2_PRIMITIVE_TRIANGLES, idx_num, GX2_INDEX_FORMAT_U16, idx, 0, 1);
}

void PrimitiveRendererCafe::drawLines_(
    const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1,
    PrimitiveRendererUtil::Vertex* vtx, u32 vtx_num, u16* idx, u32 idx_num
)
{
    GX2SetVertexUniformReg(mParamUserOffset, 12, &model_mtx);
    GX2SetVertexUniformReg(mParamColor0Offset, 4, &c0);
    GX2SetVertexUniformReg(mParamColor1Offset, 4, &c1);
    GX2SetPixelUniformReg(mParamRateOffset, 4, &Vector4f::zero);
    GX2SetAttribBuffer(0, vtx_num * 0x24, 0x24, vtx);
    GX2DrawIndexedEx(GX2_PRIMITIVE_LINE_LOOP, idx_num, GX2_INDEX_FORMAT_U16, idx, 0, 1);
}

}
