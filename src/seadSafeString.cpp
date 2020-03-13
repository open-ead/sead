#include <sead.h>

namespace sead {

template <typename T>
const T SafeStringBase<T>::cNullChar = 0;

template <typename T>
const T SafeStringBase<T>::cLineBreakChar = static_cast<T>('\n');

template <typename T>
const T SafeStringBase<T>::cNullString[1] = { SafeStringBase<T>::cNullChar };

template <typename T>
const SafeStringBase<T> SafeStringBase<T>::cEmptyString;

template <>
s32 BufferedSafeStringBase<char>::formatImpl_(char* s, s32 n, const char* formatStr, va_list args)
{
    return StringUtil::vsnprintf(s, n, formatStr, args);
}

template <>
s32 BufferedSafeStringBase<char16>::formatImpl_(char16* s, s32 n, const char16* formatStr, va_list args)
{
    return StringUtil::vsnw16printf(s, n, formatStr, args);
}

template <>
s32 BufferedSafeStringBase<char>::formatV(const char* formatStr, va_list args)
{
    char* mutableString = getMutableStringTop_();
    return formatImpl_(mutableString, mBufferSize, formatStr, args);
}

template <>
s32 BufferedSafeStringBase<char16>::formatV(const char16* formatStr, va_list args)
{
    char16* mutableString = getMutableStringTop_();
    return formatImpl_(mutableString, mBufferSize, formatStr, args);
}

template <>
s32 BufferedSafeStringBase<char>::format(const char* formatStr, ...)
{
    va_list args;
    va_start(args, formatStr);
    s32 ret = formatV(formatStr, args);
    va_end(args);

    return ret;
}

template <>
s32 BufferedSafeStringBase<char16>::format(const char16* formatStr, ...)
{
    va_list args;
    va_start(args, formatStr);
    s32 ret = formatV(formatStr, args);
    va_end(args);

    return ret;
}

} // namespace sead
