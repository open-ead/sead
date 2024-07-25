#pragma once

#include <framework/seadGameFramework.h>
#include <gfx/seadFrameBuffer.h>
#include <math/seadVector.h>
#include <nvn/nvn.h>
#include <thread/seadThread.h>

namespace sead
{
class DisplayBufferNvn;
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

class GameFrameworkNx : public GameFramework
{
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, GameFramework)
public:
    struct CreateArg
    {
        s32 mVBlankWaitInterval;
        char _8[68];
    };

    static_assert(sizeof(CreateArg) == 0x48, "");

    GameFrameworkNx(const CreateArg&);
    ~GameFrameworkNx() override;

    void initializeArg(const Framework::InitializeArg&);
    void initializeGraphicsSystem(Heap*, const Vector2f&);
    void outOfMemoryCallback_(NVNcommandBuffer*, NVNcommandBufferMemoryEvent, size_t, void*);
    void presentAsync_(Thread*, long);
    void getAcquiredDisplayBufferTexture() const;  // unknown return type
    void setVBlankWaitInterval(u32);
    void requestChangeUseGPU(bool);
    void getGraphicsDevToolsAllocatorTotalFreeSize() const;  // unknown return type
    void initRun_(Heap*) override;
    void runImpl_() override;
    MethodTreeMgr* createMethodTreeMgr_(Heap*) override;
    void mainLoop_();
    void procFrame_();
    void procDraw_();
    void procCalc_();
    void present_();
    void waitVsyncEvent_();
    void swapBuffer_();
    void clearFrameBuffers_(int);
    void waitForGpuDone_();
    void setGpuTimeStamp_();  // unknown return type
    FrameBuffer* getMethodFrameBuffer(int) const override;
    LogicalFrameBuffer* getMethodLogicalFrameBuffer(int) const override;
    float calcFps();
    void setCaption(const SafeString&);

private:
    CreateArg mCreateArg;
    char _E8[8];
    u64 mSystemTick;
    FrameBuffer* mMethodFrameBuffer;
    LogicalFrameBuffer mMethodLogicalFrameBuffer;
    char _120[8];
    DisplayBufferNvn* mDisplayBuffer;
    char _130[72];
    nn::mem::StandardAllocator* mGraphicsDevToolsAllocator;
    char _180[40];
    nn::vi::Layer* mDisplay;
    char _1B0[96];
};

static_assert(sizeof(GameFrameworkNx) == 0x210, "");

}  // namespace sead
