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
class Display;
}  // namespace vi
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
        Vector2u mDisplayResolution;
        bool _1c;
        bool mIsTripleBuffer;
        bool mIsUseDebug;
        bool _1f;
        u32 _20;
        u32 _24;
        u32 _28;
        s32 mTransferMemorySize;
        s32 mGraphicsDevToolsMemorySize;
        s32 mComputeMemorySize;
        s32 mCommandMemorySize;
        s32 mControlMemorySize;
        u32 mDebugLevel;
        u32 _44;
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
    virtual void setGpuTimeStamp_();

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
    u64 mVBlankWaitTicks;
    DisplayBufferNvn* mDisplayBuffer;
    char _130[8];
    void* mGpuErrorCallback;
    NVNmemoryPool* _140;
    void* mCommandBufferControl;
    NVNcommandBuffer* mCommandBuffer;
    NVNbuffer* _158;
    NVNcounterData* _160;
    NVNmemoryPool* mShaderScratchMemory;
    u32 _170;
    nn::mem::StandardAllocator* mGraphicsDevToolsAllocator;
    u64 mCommandBufferCommandMemoryUsed;
    u64 mCommandBufferControlMemoryUsed;
    NVNqueue* _190;
    FrameBuffer* mCurFrameBuffer;
    nn::vi::Display* mDisplay;
    nn::vi::Layer* mLayer;
    DelegateThread* mPresentationThread;
    NVNsync* _1B8;
    SafeString _1C0;
    NVNcommandHandle mCommandHandle;
    nn::os::SystemEventType mVsyncEvent;
    bool _208;
    u8 _209;
    bool _20a;
    u8 _20b;
};

static_assert(sizeof(GameFrameworkNx) == 0x210);

}  // namespace sead
