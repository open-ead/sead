#include <heap/seadExpHeap.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <thread/seadThread.h>
#include <time/seadTickSpan.h>
#include <utility>
#include "prim/seadScopedLock.h"

namespace sead
{
HeapMgr* HeapMgr::sInstancePtr = nullptr;

HeapMgr HeapMgr::sInstance;
Arena HeapMgr::sDefaultArena;
HeapMgr::RootHeaps HeapMgr::sRootHeaps;
HeapMgr::IndependentHeaps HeapMgr::sIndependentHeaps;
CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::HeapMgr() = default;

void HeapMgr::initialize(size_t size)
{
    sHeapTreeLockCS.lock();
    sArena = &sDefaultArena;
    sDefaultArena.initialize(size);
    initializeImpl_();
    sHeapTreeLockCS.unlock();
}

void HeapMgr::initializeImpl_()
{
    sInstance.mAllocFailedCallback = nullptr;
    sSleepSpanAtRemoveCacheFailure = TickSpan::makeFromMicroSeconds(10);
    createRootHeap_();
    sInstancePtr = &sInstance;
}

void HeapMgr::initialize(Arena* arena)
{
    sArena = arena;
    initializeImpl_();
}

void HeapMgr::createRootHeap_()
{
    auto* expHeap = ExpHeap::tryCreate(sArena->mStart, sArena->mSize, "RootHeap", false);
    sRootHeaps.pushBack(expHeap);
}

// NON_MATCHING: mismatching on the loops, maybe due to an issue in popBack
void HeapMgr::destroy()
{
    sHeapTreeLockCS.lock();
    sInstance.mAllocFailedCallback = nullptr;

    while (!sIndependentHeaps.isEmpty())
    {
        sIndependentHeaps.back()->destroy();
        sIndependentHeaps.popBack();
    }

    while (!sRootHeaps.isEmpty())
    {
        sRootHeaps.back()->destroy();
        sRootHeaps.popBack();
    }

    sInstancePtr = nullptr;
    sArena->destroy();
    sArena = nullptr;
    sHeapTreeLockCS.unlock();
}

void HeapMgr::initHostIO() {}

// NON_MATCHING: some mismatches during atomicHeap-search and final unlocks of mutex
Heap* HeapMgr::findContainHeap(const void* ptr) const
{
    FindContainHeapCache* cache;
    Thread* v3;
    if (ThreadMgr::instance() && (v3 = ThreadMgr::instance()->getCurrentThread()) != nullptr)
    {
        Heap* threadHeap = v3->getCurrentHeap();
        cache = v3->getFindContainHeapCache();
        Heap* heap = cache->tryAddHeap(ptr);
        bool v8 = true;
        Heap* v7 = nullptr;
        if (heap && !heap->mChildren.size())
        {
            bool includes = heap->isInclude(ptr);
            v8 = !includes;
            if (!includes)
                v7 = heap;
        }
        cache->removeHeap();
        if (!v8)
            return heap;
        if (threadHeap && threadHeap != v7 && !threadHeap->mChildren.size())
        {
            if (threadHeap->isInclude(ptr))
            {
                cache->setHeap(threadHeap);
                return threadHeap;
            }
            threadHeap = nullptr;
        }

        sHeapTreeLockCS.lock();
        Heap* atomicHeap = cache->getHeap();
        if (atomicHeap)
        {
            if (atomicHeap != v7)
            {
                Heap* containHeap = atomicHeap->findContainHeap_(ptr);
                v7 = atomicHeap;
                if (containHeap)
                {
                    Heap* resultHeap;
                    if (containHeap != atomicHeap)
                    {
                        cache->setHeap(containHeap);
                        resultHeap = containHeap;
                    }
                    else
                    {
                        resultHeap = atomicHeap;
                    }
                    sHeapTreeLockCS.unlock();
                    return resultHeap;
                }
            }
        }
        if (threadHeap)
        {
            if (threadHeap != v7)
            {
                Heap* containHeap = threadHeap->findContainHeap_(ptr);
                if (containHeap)
                {
                    sHeapTreeLockCS.unlock();
                    return containHeap;
                }
            }
        }
    }
    else
    {
        sHeapTreeLockCS.lock();
        cache = nullptr;
    }

    Heap* containHeap;
    for (Heap& heap : sRootHeaps)
    {
        containHeap = heap.findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            if (cache)
                cache->setHeap(containHeap);
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    for (Heap& heap : sIndependentHeaps)
    {
        containHeap = heap.findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            if (cache)
                cache->setHeap(containHeap);
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    sHeapTreeLockCS.unlock();
    return NULL;
}

bool HeapMgr::isContainedInAnyHeap(const void* ptr)
{
    for (auto& heap : sRootHeaps)
    {
        if (heap.isInclude(ptr))
            return true;
    }
    for (auto& heap : sIndependentHeaps)
    {
        if (heap.isInclude(ptr))
            return true;
    }
    return false;
}
void HeapMgr::dumpTreeYAML(WriteStream& stream)
{
    sHeapTreeLockCS.lock();
    for (auto& heap : sRootHeaps)
    {
        heap.dumpTreeYAML(stream, 0);
    }
    for (auto& heap : sIndependentHeaps)
    {
        heap.dumpTreeYAML(stream, 0);
    }
    sHeapTreeLockCS.unlock();
}

void HeapMgr::setAllocFromNotSeadThreadHeap(Heap* heap)
{
    mAllocFromNotSeadThreadHeap = heap;
}

void HeapMgr::removeFromFindContainHeapCache_(Heap* heap)
{
    auto* threadMgr = ThreadMgr::instance();
    if (!threadMgr)
        return;

    Thread* mainThread = threadMgr->getMainThread();
    if (mainThread)
    {
        while (!mainThread->getFindContainHeapCache()->tryRemoveHeap(heap))
        {
            Thread::sleep(sSleepSpanAtRemoveCacheFailure);
        }
    }

    while (threadMgr->tryRemoveFromFindContainHeapCache(heap))
    {
        Thread::sleep(sSleepSpanAtRemoveCacheFailure);
    }
}

Heap* HeapMgr::findHeapByName(const sead::SafeString& name, int index) const
{
    ScopedLock<CriticalSection> lock(&sHeapTreeLockCS);
    for (auto& heap : sRootHeaps)
    {
        Heap* found = findHeapByName_(&heap, name, &index);
        if (found)
            return found;
    }
    for (auto& heap : sIndependentHeaps)
    {
        Heap* found = findHeapByName_(&heap, name, &index);
        if (found)
            return found;
    }
    return nullptr;
}

Heap* HeapMgr::findHeapByName_(Heap* heap, const SafeString& name, int* index)
{
    if (heap->getName() == name)
    {
        if (*index == 0)
            return heap;
        --*index;
    }
    for (auto& child : heap->mChildren)
    {
        Heap* found = findHeapByName_(&child, name, index);
        if (found)
            return found;
    }
    return nullptr;
}

Heap* HeapMgr::getCurrentHeap() const
{
    Thread* currentThread = ThreadMgr::instance()->getCurrentThread();
    if (currentThread)
        return currentThread->getCurrentHeap();
    return mAllocFromNotSeadThreadHeap;
}

Heap* HeapMgr::setCurrentHeap_(Heap* heap)
{
    return ThreadMgr::instance()->getCurrentThread()->setCurrentHeap(heap);
}

void HeapMgr::removeRootHeap(Heap* heap)
{
    if (sRootHeaps.size() < 1)
        return;
    s32 index = sRootHeaps.indexOf(heap);
    if (index != -1)
        sRootHeaps.erase(index);
}

HeapMgr::IAllocFailedCallback
HeapMgr::setAllocFailedCallback(HeapMgr::IAllocFailedCallback callback)
{
    return std::exchange(mAllocFailedCallback, callback);
}

FindContainHeapCache::FindContainHeapCache() = default;

bool FindContainHeapCache::tryRemoveHeap(Heap* heap)
{
    uintptr_t original;
    if (mHeap.compareExchange(uintptr_t(heap), 0, &original))
        return true;
    return (original & ~1u) != uintptr_t(heap);
}
Heap* FindContainHeapCache::tryAddHeap(const void* ptr)
{
    mHeap |= 1;
    return reinterpret_cast<Heap*>(mHeap.load());
}
Heap* FindContainHeapCache::getHeap() const
{
    return reinterpret_cast<Heap*>(mHeap.load());
}
}  // namespace sead
