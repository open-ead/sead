#ifndef SEAD_SAFE_STRING_H_
#define SEAD_SAFE_STRING_H_

#include <stdarg.h>

namespace sead {

template <typename T>
class SafeStringBase
{
public:
    SafeStringBase() : mStringTop(cNullString) { }
    SafeStringBase(const T* str) : mStringTop(str) { }
    virtual ~SafeStringBase() { }

    virtual void assureTerminationImpl_() { }

    static const T cNullChar;
    static const T cNullString[1];
    static const T cLineBreakChar;
    static const SafeStringBase cEmptyString;
    static const s32 cMaximumLength = 0x40000;

    const T* mStringTop;
};

template<typename T>
const T SafeStringBase<T>::cNullChar = 0;

template<typename T>
const T SafeStringBase<T>::cLineBreakChar = static_cast<T>('\n');

template<typename T>
const T SafeStringBase<T>::cNullString[1] = { SafeStringBase<T>::cNullChar };

template<typename T>
const SafeStringBase<T> SafeStringBase<T>::cEmptyString;

template <typename T>
class BufferedSafeStringBase : public SafeStringBase<T>
{
public:
    BufferedSafeStringBase(const T* buffer, s32 size)
    {
        mStringTop = buffer;
        mBufferSize = size;
        assureTerminationImpl_();
    }

    virtual ~BufferedSafeStringBase() { }

    virtual void assureTerminationImpl_()
    {
        BufferedSafeStringBase<T>* mutableSafeString = const_cast<BufferedSafeStringBase<T>*>(this);
        getMutableStringTop_()[mBufferSize - 1] = mutableSafeString->cNullChar;
    };

    void formatV(T const*, va_list);
    void format(T const*, ...);
    void appendWithFormatV(T const*, va_list);
    void appendWithFormat(T const*, ...);

    inline T* getMutableStringTop_()
    {
        return const_cast<T*>(mStringTop);
    }

    inline void clear()
    {
        getMutableStringTop_()[0] = this->cNullChar;
    }

    s32 mBufferSize;
};

template <typename T, s32 L>
class FixedSafeStringBase : public BufferedSafeStringBase<T>
{
public:
    FixedSafeStringBase() : BufferedSafeStringBase(mBuffer, L)
    {
        clear();
    }

    virtual ~FixedSafeStringBase() { }

    T mBuffer[L];
};

typedef SafeStringBase<char> SafeString;
typedef BufferedSafeStringBase<char> BufferedSafeString;

template <s32 L>
typedef FixedSafeStringBase<char, L> FixedSafeString<L>;

template <s32 L>
class FormatFixedSafeString : public FixedSafeString<L>
{
public:
    FormatFixedSafeString() : FixedSafeStringBase() { }
    FormatFixedSafeString(const char* str, ...);
    virtual ~FormatFixedSafeString() { }
};

} // namespace sead

#endif // SEAD_SAFE_STRING_H_
