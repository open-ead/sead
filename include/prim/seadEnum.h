#pragma once

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>
#include <container/seadSafeArray.h>
#include <prim/seadSafeString.hpp>
#include <prim/seadScopedLock.h>
#include <thread/seadCriticalSection.h>

namespace sead
{
class EnumUtil
{
public:
    static void parseText_(char** text_ptr, char* text_all, int size);

    static CriticalSection* getParseTextCS_();
    static CriticalSection* getInitValueArrayCS_();

private:
    static void skipToWordEnd_(char** p_ptr, char** p_next);
    static void skipToWordStart_(char** p_ptr);
};
}  // namespace sead

/// Define an enum class. Use SEAD_ENUM_VALUES and define the enum values **in the same order**.
///
/// Example:
///
/// SEAD_ENUM(AssetType, Wave, Stream, Unknown)
/// or SEAD_ENUM(AssetType, Wave = 0, Stream = 1, Unknown = 0xFF)
///
/// followed by
///
/// SEAD_ENUM_VALUES(AssetType, 0, 1, 0xFF)
/// or SEAD_ENUM_VALUES(AssetType, Wave, Stream, Unknown)
///
/// Finally, in the .cpp file, add SEAD_ENUM_IMPL(AssetType)
///
/// For the common case where enumerators do not require custom values, use SEAD_ENUM_SIMPLE.
///
#define SEAD_ENUM(NAME, ...)                                                                       \
    class NAME                                                                                     \
    {                                                                                              \
    public:                                                                                        \
        enum ValueType                                                                             \
        {                                                                                          \
            __VA_ARGS__                                                                            \
        };                                                                                         \
                                                                                                   \
        NAME() = default;                                                                          \
        NAME(ValueType value)                                                                      \
        {                                                                                          \
            int idx = findRelativeIndex_(value);                                                   \
            if (idx == -1)                                                                         \
            {                                                                                      \
                SEAD_ASSERT(false, "could not convert to relative index: %d", idx);                \
                idx = 0;                                                                           \
            }                                                                                      \
            setRelativeIndex(idx);                                                                 \
        }                                                                                          \
        NAME(int idx) { setRelativeIndex(idx); }                                                   \
                                                                                                   \
        NAME& operator=(const NAME& other) = default;                                              \
        bool operator==(const NAME& rhs) const { return mIdx == rhs.mIdx; }                        \
                                                                                                   \
        ValueType value() const volatile { return static_cast<ValueType>(getArray_()(mIdx)); }     \
        operator int() const volatile { return value(); }                                          \
                                                                                                   \
        bool fromText(const sead::SafeString& name)                                                \
        {                                                                                          \
            for (int i = 0; i < size(); ++i)                                                       \
            {                                                                                      \
                if (name.isEqual(text(i)))                                                         \
                {                                                                                  \
                    mIdx = i;                                                                      \
                    return true;                                                                   \
                }                                                                                  \
            }                                                                                      \
            return false;                                                                          \
        }                                                                                          \
        const char* text() const { return text(mIdx); }                                            \
        static const char* text(int idx) { return text_(idx); }                                    \
                                                                                                   \
        int getRelativeIndex() const { return mIdx; }                                              \
        void setRelativeIndex(int idx)                                                             \
        {                                                                                          \
            SEAD_ASSERT(idx < size(), "range over: %d, [%d - %d)", idx, 0, size());                \
            mIdx = idx;                                                                            \
        }                                                                                          \
        static int findRelativeIndex(ValueType value) { return findRelativeIndex_(value); }        \
                                                                                                   \
        const char* getTypeText() const { return #NAME; }                                          \
        int size() const { return cCount; }                                                        \
        int getSize() const { return size(); }                                                     \
                                                                                                   \
        static void initialize() { text(0); }                                                      \
                                                                                                   \
        class iterator                                                                             \
        {                                                                                          \
        public:                                                                                    \
            iterator(int idx) : mIdx(idx) {}                                                       \
            bool operator==(const iterator& rhs) const { return mIdx == rhs.mIdx; }                \
            bool operator!=(const iterator& rhs) const { return mIdx != rhs.mIdx; }                \
            iterator& operator++()                                                                 \
            {                                                                                      \
                ++mIdx;                                                                            \
                return *this;                                                                      \
            }                                                                                      \
            iterator& operator--()                                                                 \
            {                                                                                      \
                --mIdx;                                                                            \
                return *this;                                                                      \
            }                                                                                      \
            NAME operator*() const { return NAME(mIdx); }                                          \
                                                                                                   \
        private:                                                                                   \
            int mIdx;                                                                              \
        };                                                                                         \
                                                                                                   \
        iterator begin() { return iterator(0); }                                                   \
        iterator end() { return iterator(getArray_().size()); }                                    \
                                                                                                   \
    private:                                                                                       \
        class ValueArray                                                                           \
        {                                                                                          \
        public:                                                                                    \
            ValueArray();                                                                          \
                                                                                                   \
            s32 size() const { return mSize; }                                                     \
                                                                                                   \
            int& operator()(s32 idx) { return *get(idx); }                                         \
            const int& operator()(s32 idx) const { return *get(idx); }                             \
                                                                                                   \
            int* get(s32 idx)                                                                      \
            {                                                                                      \
                if (mSize <= u32(idx))                                                             \
                {                                                                                  \
                    SEAD_ASSERT(false, "index exceeded [%d/%d]", idx, mSize);                      \
                    return mBuffer;                                                                \
                }                                                                                  \
                return &mBuffer[idx];                                                              \
            }                                                                                      \
                                                                                                   \
            const int* get(s32 idx) const                                                          \
            {                                                                                      \
                if (mSize <= u32(idx))                                                             \
                {                                                                                  \
                    SEAD_ASSERT(false, "index exceeded [%d/%d]", idx, mSize);                      \
                    return mBuffer;                                                                \
                }                                                                                  \
                return &mBuffer[idx];                                                              \
            }                                                                                      \
                                                                                                   \
        private:                                                                                   \
            s32 mSize = 0;                                                                         \
            int* mBuffer = nullptr;                                                                \
        };                                                                                         \
        friend class ValueArray;                                                                   \
        static ValueArray& getArray_();                                                            \
                                                                                                   \
        /* Returns nullptr when not found. */                                                      \
        static const char* text_(int idx);                                                         \
        /* Returns -1 when not found. */                                                           \
        static int findRelativeIndex_(ValueType value);                                            \
                                                                                                   \
        static constexpr const char* cTextAll = #__VA_ARGS__;                                      \
        static constexpr size_t cTextAllLen = sizeof(#__VA_ARGS__);                                \
        static constexpr int cCount = [] {                                                         \
            int count = 1;                                                                         \
            for (int i = 0; i < cTextAllLen; ++i)                                                  \
            {                                                                                      \
                if (cTextAll[i] == ',')                                                            \
                    ++count;                                                                       \
            }                                                                                      \
            return count;                                                                          \
        }();                                                                                       \
                                                                                                   \
        int mIdx = 0;                                                                              \
    };

/// For use with SEAD_ENUM. Use immediately after SEAD_ENUM.
#define SEAD_ENUM_VALUES(NAME, ...)                                                                \
    NAME::ValueArray::ValueArray()                                                                 \
    {                                                                                              \
        sead::ScopedLock<sead::CriticalSection> lock(sead::EnumUtil::getInitValueArrayCS_());      \
        if (mBuffer)                                                                               \
            return;                                                                                \
        struct Array : sead::SafeArray<int, NAME::cCount>                                          \
        {                                                                                          \
            Array() : sead::SafeArray<int, NAME::cCount>{__VA_ARGS__} {}                           \
        };                                                                                         \
        static Array sArray{};                                                                     \
        mSize = sArray.size();                                                                     \
        mBuffer = sArray.getBufferPtr();                                                           \
    }

/// Define an enum class.
///
/// Simpler version of SEAD_ENUM. Does not support custom enumerator values!
///
/// Example: SEAD_ENUM_SIMPLE(AssetType, Wave, Stream, Unknown)
/// followed by SEAD_ENUM_IMPL in a .cpp file
#define SEAD_ENUM_SIMPLE(NAME, ...)                                                                \
    SEAD_ENUM(NAME, __VA_ARGS__)                                                                   \
    SEAD_ENUM_VALUES(NAME, __VA_ARGS__)

/// Use this in the .cpp file.
#define SEAD_ENUM_IMPL(NAME)                                                                       \
    NAME::ValueArray& NAME::getArray_()                                                            \
    {                                                                                              \
        static ValueArray sBuffer;                                                                 \
        return sBuffer;                                                                            \
    }                                                                                              \
                                                                                                   \
    const char* NAME::text_(int idx)                                                               \
    {                                                                                              \
        if (u32(idx) >= cCount)                                                                    \
            return nullptr;                                                                        \
                                                                                                   \
        static char** spTextPtr = nullptr;                                                         \
        if (spTextPtr)                                                                             \
            return spTextPtr[idx];                                                                 \
        {                                                                                          \
            sead::ScopedLock<sead::CriticalSection> lock(sead::EnumUtil::getParseTextCS_());       \
            if (!spTextPtr)                                                                        \
            {                                                                                      \
                static char* sTextPtr[cCount];                                                     \
                static sead::FixedSafeString<cTextAllLen> sTextAll = sead::SafeString(cTextAll);   \
                sead::EnumUtil::parseText_(sTextPtr, sTextAll.getBuffer(), cCount);                \
                spTextPtr = sTextPtr;                                                              \
            }                                                                                      \
        }                                                                                          \
        return spTextPtr[idx];                                                                     \
    }                                                                                              \
                                                                                                   \
    int NAME::findRelativeIndex_(NAME::ValueType value)                                            \
    {                                                                                              \
        ValueArray& array = getArray_();                                                           \
        if (array.size() < 1)                                                                      \
            return -1;                                                                             \
                                                                                                   \
        for (int i = 0; i < array.size(); ++i)                                                     \
        {                                                                                          \
            if (array(i) == value)                                                                 \
                return i;                                                                          \
        }                                                                                          \
        return -1;                                                                                 \
    }
