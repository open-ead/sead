#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskMgr.h>
#include <heap/seadExpHeap.h>
#include <heap/seadHeap.h>
#include <random/seadGlobalRandom.h>

namespace sead
{
Framework::CreateSystemTaskArg::CreateSystemTaskArg()
    : hostio_parameter(NULL), infloop_detection_span()
{
}

void Framework::initialize(const InitializeArg& arg)
{
    if (arg.arena)
        HeapMgr::initialize(arg.arena);
    else
        HeapMgr::initialize(arg.heap_size);

    Heap* heap = HeapMgr::instance()->getRootHeap(0);

    {
        Heap* threadHeap = ExpHeap::create(0, "sead::ThreadMgr", heap);

        ThreadMgr::createInstance(threadHeap);
        ThreadMgr::instance()->initialize(threadHeap);

        threadHeap->adjust();
    }

    GlobalRandom::createInstance(heap);
}

Framework::Framework()
    : mReserveReset(false), mResetParameter(NULL), mResetEvent(), mTaskMgr(NULL),
      mMethodTreeMgr(NULL), mMethodTreeMgrHeap(NULL)
{
}

Framework::~Framework()
{
    if (mTaskMgr != NULL)
    {
        mTaskMgr->finalize();
        delete mTaskMgr;
        mTaskMgr = NULL;
    }

    if (mMethodTreeMgr != NULL)
    {
        delete mMethodTreeMgr;
        mMethodTreeMgr = NULL;
    }

    if (mMethodTreeMgrHeap != NULL)
        mMethodTreeMgrHeap->destroy();
}

}  // namespace sead
