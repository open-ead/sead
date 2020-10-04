#pragma once

#include <utility>
#include "basis/seadTypes.h"

namespace sead
{
/// Provides suitably aligned uninitialized storage for a type T.
/// Use this as a std::aligned_storage replacement that is easier to use and less error prone for
/// common cases (std::aligned_storage_t<sizeof(T), alignof(T)>).
template <typename T>
class StorageFor
{
public:
    constexpr StorageFor() = default;

    T* constructDefault() { return new (storage()) T; }

    template <typename... Args>
    T* construct(Args&&... args)
    {
        return new (storage()) T(std::forward<Args>(args)...);
    }

    /// @warning It is undefined behavior to call this if no object has been constructed.
    void destruct() { data()->~T(); }

    /// @warning It is undefined behavior to call this if no object has been constructed.
    T* data() { return reinterpret_cast<T*>(mStorage); }

    /// @warning It is undefined behavior to call this if no object has been constructed.
    const T* data() const { return reinterpret_cast<const T*>(mStorage); }

    void* storage() { return mStorage; }
    const void* storage() const { return mStorage; }

private:
    alignas(T) u8 mStorage[sizeof(T)];
};
}  // namespace sead
