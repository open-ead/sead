#pragma once

#include <utility>

namespace sead
{
template <typename T>
class ScopedLock
{
public:
    explicit ScopedLock(T* lock) : mLocked(lock) { mLocked->lock(); }

    ScopedLock(const ScopedLock& other) = delete;
    ScopedLock& operator=(const ScopedLock& other) = delete;

    ScopedLock(ScopedLock&& other) noexcept { *this = std::move(other); }
    ScopedLock& operator=(ScopedLock&& other) noexcept
    {
        if (this == &other)
            return *this;

        mLocked = other.mLocked;
        mEngaged = std::exchange(other.mEngaged, false);
        return *this;
    }

    virtual ~ScopedLock()
    {
        if (mEngaged)
            mLocked->unlock();
    }

protected:
    bool mEngaged = true;
    T* mLocked;
};

template <typename T>
[[nodiscard]] inline ScopedLock<T> makeScopedLock(T& lock)
{
    return ScopedLock<T>{&lock};
}

template <typename T>
class ConditionalScopedLock
{
public:
    ConditionalScopedLock(T* lock, bool do_lock)
    {
        if (!do_lock)
            return;
        mLocked = lock;
        mLocked->lock();
    }

    ConditionalScopedLock(const ConditionalScopedLock& other) = delete;
    ConditionalScopedLock& operator=(const ConditionalScopedLock& other) = delete;

    ConditionalScopedLock(ConditionalScopedLock&& other) noexcept { *this = std::move(other); }
    ConditionalScopedLock& operator=(ConditionalScopedLock&& other) noexcept
    {
        if (this == &other)
            return *this;

        mLocked = other.mLocked;
        mEngaged = std::exchange(other.mEngaged, false);
        return *this;
    }

    virtual ~ConditionalScopedLock()
    {
        if (mEngaged && mLocked)
            mLocked->unlock();
    }

protected:
    bool mEngaged = true;
    T* mLocked = nullptr;
};

template <typename T>
[[nodiscard]] inline ConditionalScopedLock<T> makeScopedLock(T& lock, bool do_lock)
{
    return ConditionalScopedLock<T>(&lock, do_lock);
}
}  // namespace sead
