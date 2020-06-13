#pragma once

#include <cstring>

#include <basis/seadTypes.h>

namespace sead
{
class PtrUtil
{
public:
    // Convenience function to avoid UB.
    // Nintendo doesn't care and seems to just perform type punning, but we care about UB.
    template <typename To, typename From>
    static inline To bitCast(From value)
    {
        static_assert(sizeof(To) == sizeof(From), "To and From must have the same size");
        To result;
        std::memcpy(&result, &value, sizeof(value));
        return result;
    }

    // Convenience function to avoid UB.
    // Nintendo doesn't care and seems to just perform type punning, but we care about UB.
    template <typename To>
    static inline To bitCastPtr(const void* value, intptr_t offset = 0)
    {
        To result;
        std::memcpy(&result, addOffset(value, offset), sizeof(To));
        return result;
    }

    // XXX: these probably do not match Nintendo's implementation

    static void* roundUpPow2(const void* ptr, u32 n)
    {
        const uintptr_t result = uintptr_t(ptr) + (n - uintptr_t(ptr) % n) % n;
        return reinterpret_cast<void*>(result);
    }

    static void* roundUpN(const void* ptr, u32 n)
    {
        const uintptr_t result = uintptr_t(ptr) + (n - uintptr_t(ptr) % n) % n;
        return reinterpret_cast<void*>(result);
    }

    static void* roundDownPow2(const void* ptr, u32 n)
    {
        const uintptr_t result = uintptr_t(ptr) - uintptr_t(ptr) % n;
        return reinterpret_cast<void*>(result);
    }

    static void* roundDownN(const void* ptr, u32 n)
    {
        const uintptr_t result = uintptr_t(ptr) - uintptr_t(ptr) % n;
        return reinterpret_cast<void*>(result);
    }

    static void* addOffset(const void* ptr, intptr_t offset)
    {
        return reinterpret_cast<void*>(uintptr_t(ptr) + offset);
    }

    static intptr_t diff(const void* ptr1, const void* ptr2)
    {
        return intptr_t(ptr1) - intptr_t(ptr2);
    }

    static bool isInclude(const void* ptr, const void* begin, const void* end)
    {
        return uintptr_t(begin) <= uintptr_t(ptr) && uintptr_t(ptr) <= uintptr_t(end);
    }

    static bool isAligned(const void* ptr, s32 n) { return uintptr_t(ptr) % n == 0; }
    static bool isAlignedPow2(const void* ptr, u32 n) { return uintptr_t(ptr) % n == 0; }
    static bool isAlignedN(const void* ptr, s32 n) { return uintptr_t(ptr) % n == 0; }
};
}  // namespace sead
