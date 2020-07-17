#pragma once

#ifndef SEAD_SAFE_STRING_HPP_
#error "Include <sead/seadSafeString.hpp>"
#endif

#include <cstdarg>

#include <basis/seadTypes.h>

namespace sead
{
template <typename T>
class BufferedSafeStringBase;

template <typename T>
class SafeStringBase
{
public:
    /// Iterates over every character of a string.
    /// Note that this is extremely inefficient and leads to quadratic time complexity
    /// because of the redundant calls to calcLength() in operator*.
    class iterator
    {
    public:
        explicit iterator(const SafeStringBase* string) : mString(string), mIndex(0) {}
        iterator(const SafeStringBase* string, s32 index) : mString(string), mIndex(index) {}
        bool operator==(const iterator& rhs) const
        {
            return mString == rhs.mString && mIndex == rhs.mIndex;
        }
        bool operator!=(const iterator& rhs) const { return !(rhs == *this); }
        iterator& operator++() { return mIndex++, *this; }
        iterator& operator--() { return mIndex--, *this; }
        const char& operator*() const { return mString->at(mIndex); }

        const SafeStringBase* getString() const { return mString; }
        s32 getIndex() const { return mIndex; }

    protected:
        const SafeStringBase* mString;
        s32 mIndex;
    };

    /// Iterates over a string as if it were split by one or several delimiter characters.
    class token_iterator : public iterator
    {
    public:
        token_iterator(const SafeStringBase* string, const SafeStringBase& delimiter)
            : iterator(string), mDelimiter(delimiter)
        {
        }

        token_iterator(const SafeStringBase* string, s32 index, const SafeStringBase& delimiter)
            : iterator(string, index), mDelimiter(delimiter)
        {
        }

        bool operator==(const token_iterator& rhs) const
        {
            return static_cast<const iterator&>(*this) == static_cast<const iterator&>(rhs) &&
                   mDelimiter == rhs.mDelimiter;
        }
        bool operator!=(const token_iterator& rhs) const { return !(rhs == *this); }

        token_iterator& operator++();
        token_iterator& operator--();

        s32 get(BufferedSafeStringBase<T>* out) const;
        s32 getAndForward(BufferedSafeStringBase<T>* out);
        s32 cutOffGet(BufferedSafeStringBase<T>* out) const;
        s32 cutOffGetAndForward(BufferedSafeStringBase<T>* out);

    private:
        const SafeStringBase mDelimiter;
    };

    SafeStringBase() : mStringTop(cNullString) {}
    SafeStringBase(const T* str) : mStringTop(str)
    {
        SEAD_ASSERT(str != nullptr, "str must not be nullptr.");
    }

    virtual ~SafeStringBase() = default;

    virtual SafeStringBase& operator=(const SafeStringBase& other)
    {
        mStringTop = other.mStringTop;
        return *this;
    }

    bool operator==(const SafeStringBase& rhs) const { return isEqual(rhs); }
    bool operator!=(const SafeStringBase& rhs) const { return !(rhs == *this); }

    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, calcLength()); }

    token_iterator tokenBegin(const SafeStringBase& delimiter)
    {
        return token_iterator(this, delimiter);
    }

    token_iterator tokenEnd(const SafeStringBase& delimiter)
    {
        return token_iterator(this, calcLength(), delimiter);
    }

    const T* cstr() const
    {
        assureTerminationImpl_();
        return mStringTop;
    }

    inline const T& at(s32 idx) const;
    inline const T& operator[](s32 idx) const { return at(idx); }

    inline s32 calcLength() const;

    inline SafeStringBase<T> getPart(s32 at) const;
    inline SafeStringBase<T> getPart(const iterator& it) const;
    inline SafeStringBase<T> getPart(const token_iterator& it) const;

    inline bool include(const T& c) const;
    inline bool include(const SafeStringBase<T>& str) const;

    bool isEqual(const SafeStringBase<T>& str) const;
    s32 comparen(const SafeStringBase<T>& str, s32 n) const;

    s32 findIndex(const SafeStringBase<T>& str) const { return findIndex(str, 0); }
    s32 findIndex(const SafeStringBase<T>& str, s32 start_pos) const;
    s32 rfindIndex(const SafeStringBase<T>& str) const;

    bool isEmpty() const;
    bool startsWith(const SafeStringBase<T>& prefix) const;
    bool endsWith(const SafeStringBase<T>& suffix) const;

    static const T cNullChar;
    static const T cNullString[1];
    static const T cLineBreakChar;
    static const SafeStringBase cEmptyString;
    static const s32 cMaximumLength = 0x80000;

protected:
    virtual void assureTerminationImpl_() const;
    const T& unsafeAt_(s32 idx) const { return mStringTop[idx]; }

    const T* mStringTop;
};

template <>
const char SafeStringBase<char>::cNullString[1];

template <>
const SafeStringBase<char> SafeStringBase<char>::cEmptyString;

template <typename T>
class BufferedSafeStringBase : public SafeStringBase<T>
{
public:
    __attribute__((always_inline)) BufferedSafeStringBase(T* buffer, s32 size)
        : SafeStringBase<T>(buffer)
    {
        mBufferSize = size;
        assureTerminationImpl_();
    }

    ~BufferedSafeStringBase() override = default;

    SafeStringBase<T>& operator=(const SafeStringBase<T>& other) override
    {
        copy(other);
        return *this;
    }

    void assureTerminationImpl_() const override
    {
        BufferedSafeStringBase<T>* mutableSafeString = const_cast<BufferedSafeStringBase<T>*>(this);
        mutableSafeString->getMutableStringTop_()[mBufferSize - 1] = mutableSafeString->cNullChar;
    }

    T* getBuffer() { return getMutableStringTop_(); }
    s32 getBufferSize() const { return mBufferSize; }

    s32 format(const T* formatStr, ...);
    s32 formatV(const T* formatStr, va_list args);
    s32 appendWithFormat(const T* formatStr, ...);
    s32 appendWithFormatV(const T* formatStr, va_list args);

    static s32 formatImpl_(T* s, s32 n, const T* formatStr, va_list args);

    inline T* getMutableStringTop_() { return const_cast<T*>(this->mStringTop); }

    inline void clear() { getMutableStringTop_()[0] = this->cNullChar; }

    inline s32 copy(const SafeStringBase<T>& src, s32 copyLength = -1);
    inline s32 copyAt(s32 at, const SafeStringBase<T>& src, s32 copyLength = -1);
    inline s32 cutOffCopy(const SafeStringBase<T>& src, s32 copyLength = -1);
    inline s32 cutOffCopyAt(s32 at, const SafeStringBase<T>& src, s32 copyLength = -1);
    inline s32 trim(s32 trimLength);

    s32 mBufferSize;
};

template <typename T, s32 L>
class FixedSafeStringBase : public BufferedSafeStringBase<T>
{
public:
    FixedSafeStringBase() : BufferedSafeStringBase<T>(mBuffer, L) { this->clear(); }

    FixedSafeStringBase(const SafeStringBase<T>& str) : BufferedSafeStringBase<T>(mBuffer, L)
    {
        this->copy(str);
    }

    ~FixedSafeStringBase() override = default;

    T mBuffer[L];
};

typedef SafeStringBase<char> SafeString;
typedef SafeStringBase<char16> SafeString16;
typedef BufferedSafeStringBase<char> BufferedSafeString;
typedef BufferedSafeStringBase<char16> BufferedSafeString16;

template <>
s32 BufferedSafeStringBase<char>::format(const char* formatStr, ...);
template <>
s32 BufferedSafeStringBase<char16>::format(const char16* formatStr, ...);
template <>
s32 BufferedSafeStringBase<char>::formatV(const char* formatStr, va_list args);
template <>
s32 BufferedSafeStringBase<char16>::formatV(const char16* formatStr, va_list args);
template <>
s32 BufferedSafeStringBase<char>::appendWithFormat(const char* formatStr, ...);
template <>
s32 BufferedSafeStringBase<char16>::appendWithFormat(const char16* formatStr, ...);
template <>
s32 BufferedSafeStringBase<char>::appendWithFormatV(const char* formatStr, va_list args);
template <>
s32 BufferedSafeStringBase<char16>::appendWithFormatV(const char16* formatStr, va_list args);

template <s32 L>
class FixedSafeString : public FixedSafeStringBase<char, L>
{
public:
    FixedSafeString() : FixedSafeStringBase<char, L>() {}

    FixedSafeString(const SafeString& str) : FixedSafeStringBase<char, L>(str) {}

    SafeStringBase<char>& operator=(const SafeStringBase<char>& other) override
    {
        this->copy(other);
        return *this;
    }
};

template <s32 L>
class WFixedSafeString : public FixedSafeStringBase<char16, L>
{
public:
    WFixedSafeString() : FixedSafeStringBase<char16, L>() {}

    WFixedSafeString(const SafeString16& str) : FixedSafeStringBase<char16, L>(str) {}
};

template <s32 L>
class FormatFixedSafeString : public FixedSafeStringBase<char, L>
{
public:
    FormatFixedSafeString(const char* str, ...);
    ~FormatFixedSafeString() override = default;
};

template <s32 L>
class WFormatFixedSafeString : public FixedSafeStringBase<char16, L>
{
public:
    WFormatFixedSafeString(const char16* str, ...);
    ~WFormatFixedSafeString() override = default;
};

}  // namespace sead
