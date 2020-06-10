#pragma once

#include <cafe.h>

inline void* memcpy(void* dst, const void* src, size_t len)
{
    return OSBlockMove(dst, src, len, 0);
}
