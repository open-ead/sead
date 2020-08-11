#pragma once

#include "basis/seadTypes.h"
#include "container/seadSafeArray.h"
#include "framework/seadProcessMeterBar.h"
#include "hostio/seadHostIONode.h"
#include "mc/seadCoreInfo.h"
#include "mc/seadJob.h"
#include "prim/seadEnum.h"
#include "prim/seadNamable.h"
#include "thread/seadAtomic.h"
#include "thread/seadEvent.h"
#include "container/seadBuffer.h"

namespace sead
{
class Heap;
class Worker;

SEAD_ENUM(SyncType, cNoSync, cCore, cThread)

class PerfJobQueue
{
public:
    void initialize(const char* name, Heap* heap);
    void finalize();
    void reset();

    void measureBeginDeque();
    void measureEndDeque();
    void measureBeginRun();
    void measureEndRun();

    const Color4f& getBarColor(u32 idx) const;
    void attachProcessMeter();
    void detachProcessMeter();

private:
    Buffer<MultiProcessMeterBar<512>> mBars;
    Buffer<u32> mInts;
    MultiProcessMeterBar<1> mProcessMeterBar;
};

class JobQueue : public hostio::Node, public INamable
{
public:
    enum class Status : int
    {
        _0 = 0,
        _6 = 6,
    };

    JobQueue();
    virtual ~JobQueue() = default;

    virtual void begin() {}
    virtual bool run(u32 size, u32* finished_jobs, Worker* worker);
    void runAll(u32* finished_jobs);

    virtual u32 getNumJobs() const { return 0; }
    bool isAllParticipantThrough() const;

    void setGranularity(CoreId core, u32 x);
    void setGranularity(u32);
    void setCoreMaskAndWaitType(CoreIdMask mask, SyncType type);

    void FINISH(CoreId core);
    void wait_AT_WORKER();
    void wait();

protected:
    virtual bool isDone_();

    SyncType mSyncType = SyncType::cNoSync;
    u32 mSpinLock = 0;
    CoreIdMask mMask;
    Event mEvent{true};
    SafeArray<u32, 3> mGranularity;
    SafeArray<u32, 3> mCoreEnabled;
    volatile u32 mNumDoneJobs = 0;

    Status mStatus = Status::_0;
    const char* description = "NoName";

#ifdef SEAD_DEBUG
    PerfJobQueue mPerf;
#endif
};

}  // namespace sead
