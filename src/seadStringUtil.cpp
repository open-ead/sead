#include <sead.h>

#include <stdarg.h>
#include <stdio.h>

namespace sead { namespace StringUtil {

s32 vsw16printf(char16* s, size_t n, const char16* formatStr, va_list args)
{
    if (n == 0)
        return -1;

    // Nintendo actually made their own implementation of vswprintf
    // Will use std::vswprintf for now

    // TODO: actually implement this

    return std::vswprintf(s, n, formatStr, args);
}

} } // namespace sead::StringUtil
