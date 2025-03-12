#pragma once

#include <framework/seadGameFramework.h>
#include <gfx/seadFrameBuffer.h>
#include <math/seadVector.h>
#include <nvn/nvn.h>
#include <thread/seadThread.h>

namespace nn
{
namespace mem
{
class StandardAllocator;
}
namespace vi
{
class Layer;
}
}  // namespace nn

namespace sead
{
class DisplayBufferNvn;

class GameFrameworkNx : public GameFramework
{
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, GameFramework)
public:
    struct CreateArg
    {
        s32 mVBlankWaitInterval;
        Color4f mClearColor;
        u32 _14;
        u32 _18;
        bool _1c;
        bool mIsTripleBuffer;
        bool _1e;
        bool _1f;
        u64 _20;
        u64 _28;
        u32 _30;
        s32 mComputeMemorySize;
        s32 mCommandMemorySize;
        s32 mControlMemorySize;
        u32 mDebugLevel;
    };

    static_assert(sizeof(CreateArg) == 0x48);

    GameFrameworkNx(const CreateArg&);
    ~GameFrameworkNx() override;

    FrameBuffer* getMethodFrameBuffer(int) const override;
    LogicalFrameBuffer* getMethodLogicalFrameBuffer(int) const override;
    void initRun_(Heap*) override;
    void runImpl_() override;
    MethodTreeMgr* createMethodTreeMgr_(Heap*) override;
    float calcFps() override;
    virtual void setCaption(const SafeString&);
    virtual void mainLoop_();
    virtual void procFrame_();
    virtual void procDraw_();
    virtual void procCalc_();
    virtual void present_();
    virtual void swapBuffer_();
    virtual void clearFrameBuffers_(int);
    virtual void waitForGpuDone_();
    virtual u64 setGpuTimeStamp_();  // unknown return type

    void initializeArg(const Framework::InitializeArg&);
    void initializeGraphicsSystem(Heap*, const Vector2f&);
    void outOfMemoryCallback_(NVNcommandBuffer*, NVNcommandBufferMemoryEvent, size_t, void*);
    void presentAsync_(Thread*, long);
    void getAcquiredDisplayBufferTexture() const;  // unknown return type
    void setVBlankWaitInterval(u32);
    void requestChangeUseGPU(bool);
    void getGraphicsDevToolsAllocatorTotalFreeSize() const;  // unknown return type
    void waitVsyncEvent_();

private:
    CreateArg mCreateArg;
    u64 mFrameDuration;
    u64 mPrevFrameTick;
    FrameBuffer* mMethodFrameBuffer;
    LogicalFrameBuffer mMethodLogicalFrameBuffer;
    char _120[8];
    DisplayBufferNvn* mDisplayBuffer;
    void* _130;
    void* mGpuErrorCallback;
    NVNmemoryPool* _140;
    void* mCommandBufferControl;
    NVNcommandBuffer* mCommandBuffer;
    NVNbuffer* _158;
    NVNcounterData* _160;
    NVNmemoryPool* mShaderScratchMemory;
    void* _170;
    nn::mem::StandardAllocator* mGraphicsDevToolsAllocator;
    void* _180;
    void* _188;
    NVNqueue* _190;
    agl::RenderBuffer* mCurRenderBuffer;
    nn::vi::Display* mDisplay;
    nn::vi::Layer* mLayer;
    DelegateThread* mPresentationThread;
    NVNsync* _1B8;
    SafeString _1C0;
    NVNcommandHandle mCommandHandle;
    nn::os::SystemEventType mVsyncEvent;
    void* _1F0;
    void* _1F8;
    void* _200;
    bool _208;
    bool _209;
    bool _20a;
    u8 _20b;
};

static_assert(sizeof(GameFrameworkNx) == 0x210);

}  // namespace sead
