#pragma once

#include <framework/seadGameFramework.h>
#include <math/seadVector.h>
#include <nvn/nvn.h>
#include <thread/seadThread.h>

namespace sead
{
class GameFrameworkNx : public GameFramework
{
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, GameFramework)
public:
    struct CreateArg;

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

    // missing member variables
};

}  // namespace sead
