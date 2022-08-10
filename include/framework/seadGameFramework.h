#pragma once

#include <framework/seadFramework.h>
#include <heap/seadHeap.h>
#include <math/seadVector.h>
#include <nvn/nvn_types.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>
#include <thread/seadThread.h>

namespace sead
{
class GameFramework : public Framework
{
    SEAD_RTTI_OVERRIDE(GameFramework, Framework);

public:
    GameFramework();
    // TODO: implement (missing unk1)
    ~GameFramework() override;

    void createSystemTasks(TaskBase* base,
                           const Framework::CreateSystemTaskArg& createSystemTaskArg) override;
    void quitRun_(Heap* heap) override;
    // TODO: implement (missing TaskBase::SystemMgrTaskArg, sead::TTaskFactory)
    virtual void createControllerMgr(TaskBase* base);
    virtual void createHostIOMgr(TaskBase* base, HostIOMgr::Parameter* hostioParam, Heap* heap);
    virtual void createProcessMeter(TaskBase* base);
    virtual void createSeadMenuMgr(TaskBase* base);
    virtual void createInfLoopChecker(TaskBase* base, const TickSpan&, int);
    virtual void createCuckooClock(TaskBase* base);
    virtual void saveScreenShot(const SafeString&);
    virtual bool isScreenShotBusy() const;
    virtual void waitStartDisplayLoop_();
    virtual void initHostIO_();

    void startDisplay();
    void initialize(const Framework::InitializeArg&);
    void lockFrameDrawContext();
    void unlockFrameDrawContext();

private:
    int mDisplayStarted = 0;
    sead::SafeString someString1 = "";
    sead::SafeString someString2 = "";
    sead::SafeString someString3 = "";
    void* unk1 = nullptr;
    void (*some1)(bool) = nullptr;
    void (*some2)(bool);
};
class GameFrameworkNx : public GameFramework
{
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, GameFramework)
public:
    struct CreateArg;

    GameFrameworkNx(const CreateArg&);
    ~GameFrameworkNx() override;

    void initializeArg(const Framework::InitializeArg&);
    void initializeGraphicsSystem(Heap*, const Vector2f&);
    void outOfMemoryCallback_(NVNcommandBuffer*, NVNcommandBufferMemoryEvent, ulong, void*);
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
