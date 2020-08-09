#include <atomic>

#include "basis/seadRawPrint.h"
#include "mc/seadJobQueue.h"

namespace sead
{
// NON_MATCHING
JobQueue::JobQueue()
{
    mCoreEnabled.fill(0);
    mNumDoneJobs = 0;
    mGranularity.fill(8);
}

bool JobQueue::run(u32 size, u32* finished_jobs, Worker* worker)
{
    *finished_jobs = 0;
    return true;
}

void JobQueue::runAll(u32* finished_jobs)
{
    const u32 size = getNumJobs();
    *finished_jobs = 0;
    while (true)
    {
        u32 finished_jobs_batch = 0;
        const bool ok = run(size, &finished_jobs_batch, nullptr);
        *finished_jobs += finished_jobs_batch;
        if (ok)
            break;
    }
    SEAD_ASSERT(*finished_jobs == size);
}

bool JobQueue::isAllParticipantThrough() const
{
    for (auto value : mCoreEnabled.mBuffer)
        if (value)
            return false;
    return true;
}

void JobQueue::setGranularity(CoreId core, u32 x)
{
    mGranularity[core] = x ? x : 1;
}

void JobQueue::setGranularity(u32 x)
{
    for (s32 i = 0; i < mGranularity.size(); ++i)
        setGranularity(i, x);
}

// NON_MATCHING: CMP (AND x y), #0 gets optimized into a TST; stack
void JobQueue::setCoreMaskAndWaitType(CoreIdMask mask, SyncType type)
{
    mStatus = Status::_6;
    mMask = mask;
    for (u32 i = 0; i < CoreInfo::getNumCores(); ++i)
    {
        mCoreEnabled[i] = mask.isOn(i);
        mNumDoneJobs = 0;
    }
    mSyncType = type;
}

void JobQueue::FINISH(CoreId core)
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    mCoreEnabled[core] = 0;
    wait_AT_WORKER();
}

void JobQueue::wait_AT_WORKER()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);

    switch (mSyncType)
    {
    case SyncType::WaitForWorker:
        if (!isDone_())
            mEvent.wait();
        break;
    case SyncType::_2:
        SEAD_ASSERT_MSG(false, "*NOT YET\n");
        if (!isDone_())
            mEvent.wait();
        break;
    default:
        break;
    }
}

void JobQueue::wait()
{
    if (u32(mSyncType) >= 2)
    {
        if (mSyncType != SyncType::_2)
            return;
        SEAD_ASSERT_MSG(false, "NOT IMPLEMENTED.\n");
    }
    if (!isDone_())
        mEvent.wait();
}

bool JobQueue::isDone_()
{
    return mNumDoneJobs == getNumJobs();
}
}  // namespace sead
