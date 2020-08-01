#pragma once

#include <cstring>

#include <basis/seadRawPrint.h>
#ifndef SEAD_PRIM_MEM_UTIL_H_
#include <prim/seadMemUtil.h>
#endif
#include <prim/seadPtrUtil.h>

namespace sead
{
inline void* MemUtil::fill(void* ptr, u8 c, size_t size)
{
    return std::memset(ptr, c, size);
}

inline void* MemUtil::fillZero(void* ptr, size_t size)
{
    return std::memset(ptr, 0, size);
}

inline void* MemUtil::copyOverlap(void* dest, const void* src, size_t size)
{
    return std::memmove(dest, src, size);
}

inline void* MemUtil::copy(void* dest, const void* src, size_t size)
{
    SEAD_ASSERT_MSG(!((src >= dest && PtrUtil::addOffset(dest, size) > src) ||
                      (dest >= src && PtrUtil::addOffset(src, size) > dest)),
                    "cross copy area");

    return std::memcpy(dest, src, size);
}

inline int MemUtil::compare(const void* ptr1, const void* ptr2, size_t size)
{
    return std::memcmp(ptr1, ptr2, size);
}
}  // namespace sead
