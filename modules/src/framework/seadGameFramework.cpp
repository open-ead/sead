#include <filedevice/seadFileDeviceMgr.h>
#include <framework/seadFramework.h>
#include <framework/seadGameFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadProcessMeter.h>
#include <framework/seadTaskMgr.h>
#include <gfx/seadGraphics.h>
#include <heap/seadExpHeap.h>
#include <heap/seadHeapMgr.h>
#include <resource/seadResourceMgr.h>
#include <time/seadTickSpan.h>

namespace sead
{
GameFramework::GameFramework()
{
    mUnk6 = [](bool lock) {
        if (lock)
            Graphics::instance()->lockDrawContext();
        else
            Graphics::instance()->unlockDrawContext();
    };
}

// NON_MATCHING: missing body
GameFramework::~GameFramework()
{
    // required for RTTI functions to generate
}

void GameFramework::startDisplay()
{
    if (!mDisplayStarted)
        mDisplayStarted = true;
}

void GameFramework::createSystemTasks(TaskBase* base,
                                      const Framework::CreateSystemTaskArg& createArg)
{
    CreateSystemTaskArg parentArg;
    Framework::createSystemTasks(base, parentArg);

    createControllerMgr(base);
    createProcessMeter(base);
    createSeadMenuMgr(base);
    createHostIOMgr(base, createArg.hostio_parameter, createArg.heap);
    createInfLoopChecker(base, createArg.infloop_detection_span, createArg.infloop_unk);
    createCuckooClock(base);
}

void GameFramework::createHostIOMgr([[maybe_unused]] TaskBase* base,
                                    [[maybe_unused]] HostIOMgr::Parameter* param,
                                    [[maybe_unused]] Heap* heap)
{
}

void GameFramework::createProcessMeter(TaskBase* base)
{
    ProcessMeter::createInstance(base->mHeapArray.getPrimaryHeap());
}

void GameFramework::createSeadMenuMgr([[maybe_unused]] TaskBase* base) {}

void GameFramework::createInfLoopChecker([[maybe_unused]] TaskBase* base, const TickSpan&, int) {}

void GameFramework::createCuckooClock([[maybe_unused]] TaskBase* base) {}

void GameFramework::initialize(const Framework::InitializeArg& initArg)
{
    Framework::initialize(initArg);

    Heap* firstHeap = HeapMgr::getRootHeap(0);
    ExpHeap* systemManagersHeap =
        ExpHeap::create(firstHeap->getMaxAllocatableSize(8), "sead::SystemManagers", firstHeap, 8,
                        Heap::cHeapDirection_Forward, false);

    {
        ExpHeap* resourceMgrHeap =
            ExpHeap::create(systemManagersHeap->getMaxAllocatableSize(8), "sead::ResourceMgr",
                            systemManagersHeap, 8, Heap::cHeapDirection_Forward, false);
        ScopedCurrentHeapSetter scopedHeap(resourceMgrHeap);

        ResourceMgr::createInstance(resourceMgrHeap);
        resourceMgrHeap->adjust();
    }
    {
        ExpHeap* fileDeviceMgrHeap =
            ExpHeap::create(systemManagersHeap->getMaxAllocatableSize(8), "sead::FileDeviceMgr",
                            systemManagersHeap, 8, Heap::cHeapDirection_Forward, false);
        ScopedCurrentHeapSetter scopedHeap(fileDeviceMgrHeap);

        FileDeviceMgr::createInstance(fileDeviceMgrHeap);
        fileDeviceMgrHeap->adjust();
    }

    systemManagersHeap->adjust();
}

void GameFramework::waitStartDisplayLoop_()
{
    Graphics::instance()->lockDrawContext();
    mTaskMgr->beforeCalc();
    mTaskMgr->afterCalc();
    Graphics::instance()->unlockDrawContext();

    while (mTaskMgr->mRootTaskCreateArg.factory.mType == TaskClassID::cInvalid)
    {
        if (mDisplayStarted)
            break;

        Thread::sleep(TickSpan::makeFromMilliSeconds(10));
        Graphics::instance()->lockDrawContext();
        mTaskMgr->beforeCalc();
        mTaskMgr->afterCalc();
        Graphics::instance()->unlockDrawContext();
    }

    mMethodTreeMgr->pauseAll(false);
    if (!mDisplayStarted)
        mDisplayStarted = 1;
}

void GameFramework::quitRun_([[maybe_unused]] Heap* heap) {}

void GameFramework::lockFrameDrawContext()
{
    if (mUnk5)
        mUnk5(true);
}

void GameFramework::unlockFrameDrawContext()
{
    if (mUnk5)
        mUnk5(false);
}

void GameFramework::initHostIO_() {}

void GameFramework::saveScreenShot(const SafeString&) {}

bool GameFramework::isScreenShotBusy() const
{
    return false;
}

}  // namespace sead
