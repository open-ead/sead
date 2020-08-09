#ifndef SEAD_TICKSPAN_H_
#define SEAD_TICKSPAN_H_

#include <climits>

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>

namespace sead
{
/// A TickSpan represents a number of ticks.
class TickSpan
{
public:
    TickSpan(s64 span = 0) : mSpan(span) {}

    s64 toS64() const { return mSpan; }
    s64 toTicks() const { return mSpan; }

    s64 toNanoSeconds() const;
    s64 toMicroSeconds() const { return toNanoSeconds() / 1000; }
    s64 toMilliSeconds() const { return toMicroSeconds() / 1000; }
    s64 toSeconds() const { return toMilliSeconds() / 1000; }

    void setNanoSeconds(s64 ns);
    void setMicroSeconds(s64 us) { setNanoSeconds(us * 1000); }
    void setMilliSeconds(s64 ms) { setMicroSeconds(ms * 1000); }
    void setSeconds(s64 s) { setMilliSeconds(s * 1000); }

    const TickSpan& operator+=(TickSpan span)
    {
        mSpan += span.mSpan;
        return *this;
    }

    const TickSpan& operator-=(TickSpan span)
    {
        mSpan -= span.mSpan;
        return *this;
    }

    const TickSpan& operator*=(f32 x)
    {
        mSpan *= x;
        return *this;
    }

    const TickSpan& operator/=(f32 x)
    {
        mSpan /= x;
        return *this;
    }

    friend TickSpan operator+(TickSpan a, TickSpan b) { return a.mSpan + b.mSpan; }
    friend TickSpan operator-(TickSpan a, TickSpan b) { return a.mSpan - b.mSpan; }
    friend TickSpan operator*(TickSpan a, f32 x) { return a.mSpan * x; }
    friend TickSpan operator*(f32 x, TickSpan a) { return a.mSpan * x; }
    friend TickSpan operator/(TickSpan a, f32 x) { return a.mSpan / x; }

    static TickSpan fromNanoSeconds(s64 ns)
    {
        SEAD_ASSERT(LLONG_MIN / cFrequency <= ns && ns < LLONG_MAX / cFrequency);
        TickSpan span;
        span.setNanoSeconds(ns);
        return span;
    }

    static TickSpan fromMicroSeconds(s64 us)
    {
        SEAD_ASSERT(LLONG_MIN / cFrequency <= us && us < LLONG_MAX / cFrequency);
        TickSpan span;
        span.setMicroSeconds(us);
        return span;
    }

    static TickSpan fromMilliSeconds(s64 ms)
    {
        SEAD_ASSERT(LLONG_MIN / cFrequency <= ms && ms < LLONG_MAX / cFrequency);
        TickSpan span;
        span.setMilliSeconds(ms);
        return span;
    }

    static TickSpan fromSeconds(s64 s)
    {
        SEAD_ASSERT(LLONG_MIN / cFrequency <= s && s < LLONG_MAX / cFrequency);
        TickSpan span;
        span.setSeconds(s);
        return span;
    }

private:
    s64 mSpan;

    /// Number of ticks per second. This is platform specific.
    static const s64 cFrequency;
};

}  // namespace sead

#endif  // SEAD_TICKSPAN_H_
