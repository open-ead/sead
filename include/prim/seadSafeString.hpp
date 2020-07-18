#pragma once

#define SEAD_SAFE_STRING_HPP_

#include <prim/seadMemUtil.h>
#include <prim/seadSafeString.h>

namespace sead
{
template <typename T>
inline typename SafeStringBase<T>::token_iterator& SafeStringBase<T>::token_iterator::operator++()
{
    s32 index = this->mIndex;
    const s32 length = this->mString->calcLength();
    if (0 <= index && index <= length)
    {
        while (true)
        {
            SEAD_ASSERT_NOFMT(0 <= index && index <= length);
            if (this->mString->unsafeAt_(index) == cNullChar)
                break;
            if (mDelimiter.include(this->mString->unsafeAt_(index)))
                break;
            ++index;
        }

        this->mIndex = index + 1;
    }
    else
    {
        SEAD_ASSERT(false, "index(%d) out of range [0, %d].\n", index, length);
    }
    return *this;
}

template <typename T>
inline typename SafeStringBase<T>::token_iterator& SafeStringBase<T>::token_iterator::operator--()
{
    s32 index = this->mIndex;
    const s32 length = this->mString->calcLength();

    if (index == 0)
        return *this;

    if (index == 1)
    {
        this->mIndex = 0;
        return *this;
    }

    if (0 <= index && index <= length + 1)
    {
        index -= 2;
        s32 j;
        while (true)
        {
            j = index;
            SEAD_ASSERT_NOFMT(0 <= index && index <= length);
            if (this->mString->unsafeAt_(index) == cNullChar)
                break;
            if (mDelimiter.include(this->mString->unsafeAt_(index)))
                break;
            --index;
            if (j == 0)
            {
                j = index;
                break;
            }
        }

        this->mIndex = j + 1;
    }
    else
    {
        SEAD_ASSERT(false, "index(%d) out of range [0, %d].\n", index, length + 1);
    }
    return *this;
}

template <typename T>
inline s32 SafeStringBase<T>::token_iterator::get(BufferedSafeStringBase<T>* out) const
{
    token_iterator it = *this;
    ++it;
    const s32 part_length = it.getIndex() - this->getIndex() - 1;

    const SafeStringBase<T> part = this->mString->getPart(*this);
    return out->copy(part, part_length);
}

template <typename T>
inline s32 SafeStringBase<T>::token_iterator::getAndForward(BufferedSafeStringBase<T>* out)
{
    // NOT_MATCHING(SEAD_DEBUG): minor regalloc differences.

    s32 index = this->mIndex;
    const s32 length = this->mString->calcLength();
    if (index < 0 || index > length)
    {
        SEAD_ASSERT(false, "index(%d) out of range [0, %d].\n", index, length);
        return 0;
    }

    T* outc = out->getBuffer();
    const s32 out_max_length = out->getBufferSize() - 1;

    s32 i = 0;
    while (true)
    {
        SEAD_ASSERT_NOFMT(0 <= index && index <= length);
        if (out_max_length < i)
        {
            SEAD_ASSERT(false, "token str exceeds out buffer length[%d]", out_max_length);
            return 0;
        }

        const T& c = this->mString->unsafeAt_(index);
        if (c == cNullChar || mDelimiter.include(c))
            break;

        outc[i] = c;
        ++i;
        ++index;
    }

    outc[i] = cNullChar;
    this->mIndex = index + 1;
    return i;
}

template <typename T>
inline s32 SafeStringBase<T>::token_iterator::cutOffGet(BufferedSafeStringBase<T>* out) const
{
    token_iterator it = *this;
    ++it;
    const s32 part_length = it.getIndex() - this->getIndex() - 1;

    const SafeStringBase<T> part = this->mString->getPart(this->getIndex());
    return out->cutOffCopy(part, part_length);
}

template <typename T>
inline s32 SafeStringBase<T>::token_iterator::cutOffGetAndForward(BufferedSafeStringBase<T>* out)
{
    s32 index = this->mIndex;
    const s32 length = this->mString->calcLength();
    if (index < 0 || index > length)
    {
        SEAD_ASSERT(false, "index(%d) out of range [0, %d].\n", index, length);
        return 0;
    }

    T* outc = out->getBuffer();
    const s32 out_max_length = out->getBufferSize() - 1;

    s32 i = 0;
    while (true)
    {
        SEAD_ASSERT_NOFMT(0 <= index && index <= length);

        const T& c = this->mString->unsafeAt_(index);
        if (c == cNullChar || mDelimiter.include(c))
            break;

        if (i < out_max_length)
            outc[i++] = c;
        ++index;
    }

    SEAD_ASSERT_NOFMT(i <= out_max_length);
    outc[i] = cNullChar;
    this->mIndex = index + 1;
    return i;
}

template <typename T>
inline const T& SafeStringBase<T>::at(s32 idx) const
{
    const int length = calcLength();
    if (idx >= 0 && idx < length)
        return mStringTop[idx];

    SEAD_ASSERT(false, "index(%d) out of range[0, %d]", idx, length);
    return cNullChar;
}

template <typename T>
inline SafeStringBase<T> SafeStringBase<T>::getPart(s32 at) const
{
    s32 len = calcLength();
    if (at < 0 || at > len)
    {
        SEAD_ASSERT(false, "index(%d) out of range[0, %d]", at, len);
        return SafeStringBase<T>::cEmptyString;
    }

    return SafeStringBase<T>(mStringTop + at);
}

template <typename T>
inline SafeStringBase<T> SafeStringBase<T>::getPart(const SafeStringBase::iterator& it) const
{
    return getPart(it.getIndex());
}

template <typename T>
inline SafeStringBase<T> SafeStringBase<T>::getPart(const SafeStringBase::token_iterator& it) const
{
    return getPart(it.getIndex());
}

template <typename T>
inline s32 SafeStringBase<T>::calcLength() const
{
    SEAD_ASSERT_NOFMT(mStringTop);
    assureTerminationImpl_();
    s32 length = 0;

    for (;;)
    {
        if (length > cMaximumLength || mStringTop[length] == cNullChar)
            break;

        length++;
    }

    if (length > cMaximumLength)
    {
        SEAD_ASSERT(false, "too long string");
        return 0;
    }

    return length;
}

template <typename T>
inline bool SafeStringBase<T>::include(const T& c) const
{
    assureTerminationImpl_();
    for (s32 i = 0; i <= cMaximumLength; ++i)
    {
        if (unsafeAt_(i) == cNullChar)
            break;
        if (unsafeAt_(i) == c)
            return true;
    }
    return false;
}

template <typename T>
inline bool SafeStringBase<T>::include(const SafeStringBase<T>& str) const
{
    return findIndex(str) != -1;
}

template <typename T>
inline bool SafeStringBase<T>::isEqual(const SafeStringBase<T>& str) const
{
    assureTerminationImpl_();
    if (cstr() == str.cstr())
        return true;

    for (s32 i = 0; i <= cMaximumLength; i++)
    {
        if (unsafeAt_(i) != str.unsafeAt_(i))
            return false;

        if (unsafeAt_(i) == cNullChar)
            return true;
    }

    SEAD_ASSERT(false, "too long string\n");
    return false;
}

template <typename T>
inline s32 SafeStringBase<T>::comparen(const SafeStringBase<T>& str, s32 n) const
{
    assureTerminationImpl_();
    if (cstr() == str.cstr())
        return 0;

    if (n > cMaximumLength)
    {
        SEAD_ASSERT(false, "paramater(%d) out of bounds [0, %d]", n, cMaximumLength);
        n = cMaximumLength;
    }

    for (s32 i = 0; i < n; ++i)
    {
        if (unsafeAt_(i) < str.unsafeAt_(i))
            return -1;

        if (unsafeAt_(i) > str.unsafeAt_(i))
            return 1;

        if (unsafeAt_(i) == cNullChar)
            return 0;
    }

    return 0;
}

template <typename T>
inline s32 SafeStringBase<T>::findIndex(const SafeStringBase<T>& str, s32 start_pos) const
{
    const s32 len = calcLength();

    if (start_pos < 0 || start_pos > len)
    {
        SEAD_ASSERT(false, "start_pos(%d) out of range[0, %d]", start_pos, len);
        return -1;
    }

    const s32 sub_str_len = str.calcLength();

    for (s32 i = start_pos; i <= len - sub_str_len; ++i)
    {
        if (SafeStringBase<T>(&mStringTop[i]).comparen(str, sub_str_len) == 0)
            return i;
    }
    return -1;
}

template <typename T>
inline s32 SafeStringBase<T>::rfindIndex(const SafeStringBase<T>& str) const
{
    const s32 len = calcLength();
    const s32 sub_str_len = str.calcLength();

    for (s32 i = len - sub_str_len; i >= 0; --i)
    {
        if (SafeStringBase<T>(&mStringTop[i]).comparen(str, sub_str_len) == 0)
            return i;
    }
    return -1;
}

template <typename T>
inline bool SafeStringBase<T>::isEmpty() const
{
    return unsafeAt_(0) == '\0';
}

template <typename T>
inline bool SafeStringBase<T>::startsWith(const SafeStringBase<T>& prefix) const
{
    return findIndex(prefix) == 0;
}

template <typename T>
inline bool SafeStringBase<T>::endsWith(const SafeStringBase<T>& suffix) const
{
    const s32 sub_str_len = suffix.calcLength();
    if (sub_str_len == 0)
        return true;

    const T* strc = mStringTop;
    const T* suffixc = suffix.mStringTop;

    const s32 len = calcLength();
    if (len < sub_str_len)
        return false;

    for (s32 i = 0; i < sub_str_len; ++i)
    {
        if (strc[len - sub_str_len + i] != suffixc[i])
            return false;
    }
    return true;
}

template <typename T>
inline s32 BufferedSafeStringBase<T>::copy(const SafeStringBase<T>& src, s32 copyLength)
{
    T* dst = getMutableStringTop_();
    const T* csrc = src.cstr();
    if (dst == csrc)
        return 0;

    if (copyLength < 0)
        copyLength = src.calcLength();

    if (copyLength >= mBufferSize)
    {
        SEAD_ASSERT(false, "Buffer overflow. (Buffer Size: %d, Copy Size: %d)", mBufferSize,
                    copyLength);
        copyLength = mBufferSize - 1;
    }

    MemUtil::copy(dst, csrc, copyLength * sizeof(T));
    dst[copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32 BufferedSafeStringBase<T>::copyAt(s32 at, const SafeStringBase<T>& src, s32 copyLength)
{
    T* dst = getMutableStringTop_();
    s32 len = this->calcLength();

    if (at < 0)
    {
        at = len + at + 1;
        if (at < 0)
        {
            SEAD_ASSERT(false, "at(%d) out of range[0, %d]", at, len);
            at = 0;
        }
    }

    if (copyLength < 0)
        copyLength = src.calcLength();

    if (copyLength >= mBufferSize - at)
    {
        SEAD_ASSERT(false, "Buffer overflow. (Buffer Size: %d, At: %d, Copy Length: %d)",
                    mBufferSize, at, copyLength);
        copyLength = mBufferSize - at - 1;
    }

    if (copyLength <= 0)
        return 0;

    MemUtil::copy(dst + at, src.cstr(), copyLength * sizeof(T));
    if (at + copyLength > len)
        dst[at + copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32 BufferedSafeStringBase<T>::cutOffCopy(const SafeStringBase<T>& src, s32 copyLength)
{
    T* dst = getMutableStringTop_();
    const T* csrc = src.cstr();
    if (dst == csrc)
        return 0;

    if (copyLength < 0)
        copyLength = src.calcLength();

    if (copyLength >= mBufferSize)
        copyLength = mBufferSize - 1;

    MemUtil::copy(dst, csrc, copyLength * sizeof(T));
    dst[copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32 BufferedSafeStringBase<T>::cutOffCopyAt(s32 at, const SafeStringBase<T>& src,
                                                   s32 copyLength)
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

    if (copyLength >= mBufferSize - at)
        copyLength = mBufferSize - at - 1;

    if (copyLength <= 0)
        return 0;

    MemUtil::copy(dst + at, src.cstr(), copyLength * sizeof(T));
    if (at + copyLength > len)
        dst[at + copyLength] = SafeStringBase<T>::cNullChar;

    return copyLength;
}

template <typename T>
inline s32 BufferedSafeStringBase<T>::trim(s32 trimLength)
{
    if (trimLength >= mBufferSize)
        return this->calcLength();

    if (trimLength < 0)
        trimLength = 0;

    T* mutableString = getMutableStringTop_();
    mutableString[trimLength] = SafeStringBase<T>::cNullChar;

    return trimLength;
}

}  // namespace sead
