#pragma once

#include <framework/seadFramework.h>
#include <heap/seadHeap.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

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
    sead::SafeString mUnk1 = "";
    sead::SafeString mUnk2 = "";
    sead::SafeString mUnk3 = "";
    void* mUnk4 = nullptr;
    void (*mUnk5)(bool) = nullptr;
    void (*mUnk6)(bool);
};
}  // namespace sead
