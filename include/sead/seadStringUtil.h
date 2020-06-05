#ifndef SEAD_STRING_UTIL_H_
#define SEAD_STRING_UTIL_H_

#include <stdarg.h>
#include <stdio.h>

#include <sead/types.h>

namespace sead { namespace StringUtil {

s32 vsnprintf(char* s, size_t n, const char* formatStr, va_list args);
s32 vsw16printf(char16* s, size_t n, const char16* formatStr, va_list args);
s32 vsnw16printf(char16* s, size_t n, const char16* formatStr, va_list args);

} } // namespace sead::StringUtil

#endif // SEAD_STRING_UTIL_H_
