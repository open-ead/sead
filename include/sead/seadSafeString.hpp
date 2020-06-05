#pragma once

#include <string>
#ifdef cafe
#include <sead/seadMemUtilCafe.hpp>
#endif  // cafe
#include <sead/seadSafeString.h>

namespace sead {

template <typename T>
inline s32
SafeStringBase<T>::calcLength() const
{
    assureTerminationImpl_();
    s32 length = 0;

    for (;;)
    {
        if (length > cMaximumLength || mStringTop[length] == cNullChar)
            break;

        length++;
    }

    if (length > cMaximumLength)
        return 0;

    return length;
}

template <typename T>
inline bool
SafeStringBase<T>::isEqual(const SafeStringBase<T>& str) const
{
    assureTerminationImpl_();
    if (cstr() == str.cstr())
        return true;

    for (s32 i = 0; i <= cMaximumLength; i++)
    {
        if (unsafeAt_(i) != str.unsafeAt_(i))
            return false;

        else if (unsafeAt_(i) == cNullChar)
            return true;
    }

    return false;
}

template <typename T>
inline s32
SafeStringBase<T>::comparen(const SafeStringBase<T>& str, s32 n) const
{
    assureTerminationImpl_();
    if (cstr() == str.cstr())
        return 0;

    if (n > cMaximumLength)
        n = cMaximumLength;

    for (s32 i = 0; i < n; i++)
    {
        if (unsafeAt_(i) == cNullChar && str.unsafeAt_(i) == cNullChar)
            return 0;

        else if (unsafeAt_(i) == cNullChar)
            return -1;

        else if (str.unsafeAt_(i) == cNullChar)
            return 1;

        else if (unsafeAt_(i) < str.unsafeAt_(i))
            return -1;

        else if (unsafeAt_(i) > str.unsafeAt_(i))
            return 1;
    }

    return 0;
}

template <typename T>
inline s32
SafeStringBase<T>::findIndex(const SafeStringBase<T>& str) const
{
    s32 len = calcLength();
    s32 subStrLen = str.calcLength();

    for (s32 i = 0; i <= len - subStrLen; i++)
        if (SafeStringBase<T>(&mStringTop[i]).comparen(str, subStrLen) == 0)
            return i;

    return -1;
}

template <typename T>
inline s32
BufferedSafeStringBase<T>::copy(const SafeStringBase<T>& src, s32 copyLength)
{
    T* dst = getMutableStringTop_();
    if (copyLength < 0)
        copyLength = src.calcLength();

    if (copyLength >= mBufferSize)
        copyLength = mBufferSize - 1;

    std::char_traits<T>::copy(dst, src.cstr(), copyLength);
    dst[copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32
BufferedSafeStringBase<T>::copyAt(s32 at, const SafeStringBase<T>& src, s32 copyLength)
{
    T* dst = getMutableStringTop_();
    s32 len = this->calcLength();

    if (at < 0)
    {
        at = len + at + 1;
        if (at < 0)
            at = 0;
    }

    if (copyLength < 0)
        copyLength = src.calcLength();

    if (at + copyLength >= mBufferSize)
        copyLength = mBufferSize - at - 1;

    if (copyLength <= 0)
        return 0;

    std::char_traits<T>::copy(dst + at, src.cstr(), copyLength);
    if (at + copyLength > len)
        dst[at + copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32
BufferedSafeStringBase<T>::trim(s32 trimLength)
{
    if (trimLength >= mBufferSize)
        return this->calcLength();

    if (trimLength < 0)
        trimLength = 0;

    T* mutableString = getMutableStringTop_();
    mutableString[trimLength] = SafeStringBase<T>::cNullChar;

    return trimLength;
}

} // namespace sead
