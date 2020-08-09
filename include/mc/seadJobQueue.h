#pragma once

#include "basis/seadTypes.h"
#include "container/seadSafeArray.h"
#include "framework/seadProcessMeterBar.h"
#include "hostio/seadHostIONode.h"
#include "mc/seadCoreInfo.h"
#include "mc/seadJob.h"
#include "prim/seadNamable.h"
#include "thread/seadAtomic.h"
#include "thread/seadEvent.h"

namespace sead
{
class Worker;

enum class SyncType : int
{
    _0 = 0,
    WaitForWorker = 1,
    _2 = 2,
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

    SyncType mSyncType = SyncType::_0;
    u32 mSpinLock = 0;
    CoreIdMask mMask;
    Event mEvent{true};
    SafeArray<u32, 3> mGranularity;
    SafeArray<u32, 3> mCoreEnabled;
    volatile u32 mNumDoneJobs = 0;

    Status mStatus = Status::_0;
    const char* description = "NoName";

#ifdef SEAD_DEBUG
    u32 _108 = 0;
    void* _110 = nullptr;
    u32 _118 = 0;
    void* _120 = nullptr;

    MultiProcessMeterBar<1> mProcessMeterBar{SafeString::cNullString, Color4f::cRed};
#endif
};

}  // namespace sead
