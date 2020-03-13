#ifndef SEAD_STRING_UTIL_H_
#define SEAD_STRING_UTIL_H_

#include <stdarg.h>
#include <stdio.h>

namespace sead { namespace StringUtil {

inline s32 vsnprintf(char* s, size_t n, const char* formatStr, va_list args)
{
    s32 ret = ::vsnprintf(s, n, formatStr, args);
    if (ret < 0 || ret >= n)
    {
        s[n - 1] = '\0';
        ret = n - 1;
    }

    return ret;
}

s32 vsw16printf(char16* s, size_t n, const char16* formatStr, va_list args);

inline s32 vsnw16printf(char16* s, size_t n, const char16* formatStr, va_list args)
{
    s32 ret = vsw16printf(s, n, formatStr, args);
    if (ret < 0 || ret >= n)
    {
        s[n - 1] = 0;
        ret = n - 1;
    }

    return ret;
}

} } // namespace sead::StringUtil

#endif // SEAD_STRING_UTIL_H_
