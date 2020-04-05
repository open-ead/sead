#include <sead.h>

namespace {

const u32 cDestructedFlag = 1;

} // namespace {no-name}

namespace sead {

IDisposer::IDisposer()
    : mListNode()
    , mDisposerHeap(NULL)
{
    if (sead::HeapMgr::sInstancePtr != NULL)
    {
        mDisposerHeap = sead::HeapMgr::sInstancePtr->findContainHeap(this);
        if (mDisposerHeap != NULL)
            mDisposerHeap->appendDisposer_(this);
    }
}

IDisposer::~IDisposer()
{
    if (*reinterpret_cast<size_t*>(&mDisposerHeap) != cDestructedFlag)
    {
        if (mDisposerHeap != NULL)
            mDisposerHeap->removeDisposer_(this);

        *reinterpret_cast<size_t*>(&mDisposerHeap) = cDestructedFlag;
    }
}

} // namespace sead
