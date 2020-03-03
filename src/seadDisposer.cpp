#include <sead.h>

namespace {

const u32 cDestructedFlag = 1;

} // namespace {no-name}

namespace sead {

IDisposer::IDisposer()
    : listNode()
    , containHeap(NULL)
{
    if (sead::HeapMgr::sInstancePtr != NULL)
    {
        containHeap = sead::HeapMgr::sInstancePtr->findContainHeap(this);
        if (containHeap != NULL)
            containHeap->appendDisposer_(this);
    }
}

IDisposer::~IDisposer()
{
    if (destructFlag != cDestructedFlag)
    {
        if (containHeap != NULL)
            containHeap->removeDisposer_(this);

        destructFlag = cDestructedFlag;
    }
}

} // namespace sead
