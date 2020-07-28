#include "basis/seadRawPrint.h"
#include "thread/seadEvent.h"

namespace sead
{
Event::Event() : IDisposer(), mInitialized(false) {}

Event::Event(bool manual_reset) : Event()
{
    initialize(manual_reset);
}

Event::Event(Heap* disposer_heap) : Event(disposer_heap, HeapNullOption::UseSpecifiedOrContainHeap)
{
}

Event::Event(Heap* disposer_heap, bool manual_reset) : Event(disposer_heap)
{
    initialize(manual_reset);
}

Event::Event(Heap* disposer_heap, IDisposer::HeapNullOption heap_null_option)
    : IDisposer(disposer_heap, heap_null_option), mInitialized(false)
{
}

Event::Event(Heap* disposer_heap, IDisposer::HeapNullOption heap_null_option, bool manual_reset)
    : Event(disposer_heap, heap_null_option)
{
    initialize(manual_reset);
}

Event::~Event()
{
    mInitialized = false;
    nn::os::FinalizeLightEvent(&mEventInner);
}

void Event::initialize(bool manual_reset)
{
    SEAD_ASSERT_MSG(!mInitialized, "Event is already initialized.");
    nn::os::InitializeLightEvent(&mEventInner, false, !manual_reset);
    mInitialized = true;
}

void Event::wait()
{
    SEAD_ASSERT_MSG(mInitialized, "Event is not initialized.");
    nn::os::WaitLightEvent(&mEventInner);
}

bool Event::wait(TickSpan duration)
{
    SEAD_ASSERT_MSG(mInitialized, "Event is not initialized.");
    return nn::os::TimedWaitLightEvent(&mEventInner, nn::os::ConvertToTimeSpan(duration.toTicks()));
}

void Event::setSignal()
{
    SEAD_ASSERT_MSG(mInitialized, "Event is not initialized.");
    nn::os::SignalLightEvent(&mEventInner);
}

void Event::resetSignal()
{
    SEAD_ASSERT_MSG(mInitialized, "Event is not initialized.");
    nn::os::ClearLightEvent(&mEventInner);
}
}  // namespace sead
