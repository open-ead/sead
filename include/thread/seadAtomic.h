#pragma once

#ifdef NNSDK
#include <atomic>
#include <type_traits>
#endif

namespace sead
{
template <class T>
struct AtomicBase
{
public:
    explicit AtomicBase(T value = {});

    /// Load the current value, as if with memory_order_relaxed.
    T load() const;
    /// Store a new value, as if with memory_order_relaxed.
    void store(T value);
    /// Exchange/swap the current value, as if with memory_order_relaxed.
    /// @return the previous value
    T exchange(T value);
    /// Load the current value and if it is equal to `expected`, store `desired`
    /// as if with memory_order_relaxed.
    /// Otherwise, this sets `original` to the current value.
    /// @param expected  The value expected to be found in the atomic object, and to be replaced.
    /// @param desired   The new value to store in the atomic object if `expected` was found.
    /// @param original  The value that was found in the atomic object if the comparison fails. May
    ///                  be null. Note that this is only updated when false is returned.
    /// @return true if and only if the value was modified
    bool compareExchange(T expected, T desired, T* original = nullptr);

protected:
#ifdef NNSDK
    std::atomic<T> mValue;
    static_assert(sizeof(mValue) == sizeof(T), "std::atomic<T> is larger than T; unsupported case");
#endif
};

template <class T>
struct Atomic : AtomicBase<T>
{
    using AtomicBase<T>::AtomicBase;
    T fetchAdd(T x);
    T fetchSub(T x);
    T fetchAnd(T x);
    T fetchOr(T x);
    T fetchXor(T x);
    T increment() { return fetchAdd(1); }
    T decrement() { return fetchSub(1); }
};

/// Specialization for pointer types.
template <class T>
struct Atomic<T*> : AtomicBase<T*>
{
    using AtomicBase<T*>::AtomicBase;
};

// Implementation.

#ifdef NNSDK
template <class T>
inline AtomicBase<T>::AtomicBase(T value) : mValue{value}
{
}

template <class T>
inline T AtomicBase<T>::load() const
{
    return mValue.load(std::memory_order_relaxed);
}

template <class T>
inline void AtomicBase<T>::store(T value)
{
    mValue.store(value, std::memory_order_relaxed);
}

template <class T>
inline T AtomicBase<T>::exchange(T value)
{
    return mValue.exchange(value, std::memory_order_relaxed);
}

template <class T>
inline bool AtomicBase<T>::compareExchange(T expected, T desired, T* original)
{
#ifdef MATCHING_HACK_NX_CLANG
    // Nintendo appears to have reimplemented compare_exchange_strong manually
    // without using Clang's atomic intrinsics.
    // Unlike Clang (https://reviews.llvm.org/D13033), their version does not use clrex.
    do
    {
        T value = __builtin_arm_ldrex(reinterpret_cast<volatile T*>(&mValue));
        if (value != expected)
        {
            if (original)
                *original = value;
            return false;
        }
    } while (__builtin_arm_strex(desired, reinterpret_cast<volatile T*>(&mValue)));
    return true;
#else
    T value = expected;
    if (mValue.compare_exchange_strong(value, desired, std::memory_order_relaxed))
        return true;
    if (original)
        *original = value;
    return false;
#endif
}

template <class T>
inline T Atomic<T>::fetchAdd(T x)
{
    return this->mValue.fetch_add(x, std::memory_order_relaxed);
}

template <class T>
inline T Atomic<T>::fetchSub(T x)
{
    return this->mValue.fetch_sub(x, std::memory_order_relaxed);
}

template <class T>
inline T Atomic<T>::fetchAnd(T x)
{
    return this->mValue.fetch_and(x, std::memory_order_relaxed);
}

template <class T>
inline T Atomic<T>::fetchOr(T x)
{
    return this->mValue.fetch_or(x);
}

template <class T>
inline T Atomic<T>::fetchXor(T x)
{
    return this->mValue.fetch_xor(x);
}
#else  // NNSDK
#error "Unknown platform"
#endif
}  // namespace sead
