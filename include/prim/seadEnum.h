#pragma once

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>
#include <container/seadSafeArray.h>
#include <prim/seadSafeString.h>
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

    static constexpr int countValues(const char* text_all, size_t text_all_len)
    {
        int count = 1;
        for (size_t i = 0; i < text_all_len; ++i)
        {
            if (text_all[i] == ',')
                ++count;
        }
        return count;
    }

private:
    static void skipToWordEnd_(char** p_ptr, char** p_next);
    static void skipToWordStart_(char** p_ptr);
};
}  // namespace sead

/// Define an enum class. Custom enumerator values are *not* supported.
///
/// Example:
///
/// SEAD_ENUM(CoreId, cMain, cSub1, cSub2)
///
#define SEAD_ENUM(NAME, ...)                                                                       \
    class NAME                                                                                     \
    {                                                                                              \
        /* Note: We cannot provide volatile overloads of the copy constructor and assignment       \
         * operator, because doing so would cause this class to stop being trivially copyable.     \
         * As a workaround we provide an implicit conversion to CvRef and a copy assignment        \
         * operator overload that takes a CvRef. The operator int() and NAME(int) constructor      \
         * allow construction from a const volatile NAME. */                                       \
        struct CvRef                                                                               \
        {                                                                                          \
            const NAME& asRef() const { return const_cast<const NAME&>(cvref); }                   \
                                                                                                   \
            bool is_cv;                                                                            \
            const volatile NAME& cvref;                                                            \
        };                                                                                         \
                                                                                                   \
    public:                                                                                        \
        enum ValueType                                                                             \
        {                                                                                          \
            __VA_ARGS__                                                                            \
        };                                                                                         \
                                                                                                   \
        /* This must be user-defined for correct codegen of static constructors. */                \
        NAME() : mIdx(0) {}                                                                        \
        /* NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-pro-type-member-init) */   \
        NAME(ValueType value) { setRelativeIndex(value); }                                         \
        /* NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-pro-type-member-init) */   \
        NAME(int idx) { setRelativeIndex(idx); }                                                   \
        NAME(const NAME& other) = default;                                                         \
                                                                                                   \
        NAME& operator=(const NAME& other) = default;                                              \
        NAME& operator=(ValueType value)                                                           \
        {                                                                                          \
            setRelativeIndex(value);                                                               \
            return *this;                                                                          \
        }                                                                                          \
        volatile NAME& operator=(ValueType value) volatile                                         \
        {                                                                                          \
            setRelativeIndex(value);                                                               \
            return *this;                                                                          \
        }                                                                                          \
        volatile NAME& operator=(CvRef other) volatile                                             \
        {                                                                                          \
            setRelativeIndex(other.is_cv ? other.cvref.mIdx : other.asRef().mIdx);                 \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        bool operator==(const NAME& rhs) const { return mIdx == rhs.mIdx; }                        \
        bool operator!=(const NAME& rhs) const { return mIdx != rhs.mIdx; }                        \
                                                                                                   \
        bool operator==(ValueType value) const { return ValueType(mIdx) == value; }                \
        bool operator==(ValueType value) const volatile { return ValueType(mIdx) == value; }       \
                                                                                                   \
        bool operator!=(ValueType value) const { return ValueType(mIdx) != value; }                \
        bool operator!=(ValueType value) const volatile { return ValueType(mIdx) != value; }       \
                                                                                                   \
        ValueType value() const { return static_cast<ValueType>(mIdx); }                           \
        ValueType value() const volatile { return static_cast<ValueType>(mIdx); }                  \
        /* XXX: Bafflingly, there is no purely const-qualified version of operator int().  */      \
        /* This leads to suboptimal codegen in many places. */                                     \
        operator int() const volatile { return value(); }                                          \
        operator CvRef() const { return {false, *this}; }                                          \
        operator CvRef() const volatile { return {true, *this}; }                                  \
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
        const char* text() const volatile { return text(mIdx); }                                   \
        static const char* text(int idx) { return text_(idx); }                                    \
                                                                                                   \
        int getRelativeIndex() const { return mIdx; }                                              \
        int getRelativeIndex() const volatile { return mIdx; }                                     \
        void setRelativeIndex(int idx)                                                             \
        {                                                                                          \
            SEAD_ASSERT_MSG(u32(idx) < u32(size()), "range over: %d, [%d - %d)", idx, 0, size());  \
            mIdx = idx;                                                                            \
        }                                                                                          \
        void setRelativeIndex(int idx) volatile                                                    \
        {                                                                                          \
            SEAD_ASSERT_MSG(u32(idx) < u32(size()), "range over: %d, [%d - %d)", idx, 0, size());  \
            mIdx = idx;                                                                            \
        }                                                                                          \
                                                                                                   \
        const char* getTypeText() const { return #NAME; }                                          \
        const char* getTypeText() const volatile { return #NAME; }                                 \
        constexpr static int size() { return cCount; }                                             \
        constexpr static int getSize() { return size(); }                                          \
        constexpr static int getLastIndex() { return size() - 1; }                                 \
                                                                                                   \
        static void initialize() { text(0); }                                                      \
                                                                                                   \
        class iterator                                                                             \
        {                                                                                          \
        public:                                                                                    \
            explicit iterator(int idx) : mIdx(idx) {}                                              \
            bool operator==(const iterator& rhs) const { return mIdx == rhs.mIdx; }                \
            bool operator!=(const iterator& rhs) const { return mIdx != rhs.mIdx; }                \
            iterator& operator++()                                                                 \
            {                                                                                      \
                if (mIdx <= getLastIndex())                                                        \
                {                                                                                  \
                    ++mIdx;                                                                        \
                }                                                                                  \
                else                                                                               \
                {                                                                                  \
                    SEAD_ASSERT_MSG(false, "enum iterator overflow");                              \
                    mIdx = size();                                                                 \
                }                                                                                  \
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
        static iterator begin() { return iterator(0); }                                            \
        static iterator end() { return iterator(size()); }                                         \
                                                                                                   \
    private:                                                                                       \
        /* Returns nullptr when not found. */                                                      \
        static const char* text_(int idx)                                                          \
        {                                                                                          \
            if (u32(idx) >= cCount)                                                                \
                return nullptr;                                                                    \
                                                                                                   \
            static char** spTextPtr = nullptr;                                                     \
            if (spTextPtr)                                                                         \
                return spTextPtr[idx];                                                             \
            {                                                                                      \
                sead::ScopedLock<sead::CriticalSection> lock(sead::EnumUtil::getParseTextCS_());   \
                if (!spTextPtr)                                                                    \
                {                                                                                  \
                    static char* sTextPtr[cCount];                                                 \
                    static sead::FixedSafeString<cTextAllLen> sTextAll =                           \
                        sead::SafeString(cTextAll);                                                \
                    sead::EnumUtil::parseText_(sTextPtr, sTextAll.getBuffer(), cCount);            \
                    spTextPtr = sTextPtr;                                                          \
                }                                                                                  \
            }                                                                                      \
            return spTextPtr[idx];                                                                 \
        }                                                                                          \
                                                                                                   \
        static constexpr const char* cTextAll = #__VA_ARGS__;                                      \
        static constexpr size_t cTextAllLen = sizeof(#__VA_ARGS__);                                \
        static constexpr int cCount = sead::EnumUtil::countValues(cTextAll, cTextAllLen);          \
                                                                                                   \
        int mIdx;                                                                                  \
    };

/// Define a complex enum class with custom enumerator values with this macro.
/// You must then use SEAD_ENUM_EX_VALUES and define the enum values **in the same order**.
///
/// Example:
///
/// SEAD_ENUM_EX(AssetType, Wave, Stream, Unknown)
/// or SEAD_ENUM_EX(AssetType, Wave = 0, Stream = 1, Unknown = 0xFF)
///
/// followed by
///
/// SEAD_ENUM_EX_VALUES(AssetType, 0, 1, 0xFF)
/// or SEAD_ENUM_EX_VALUES(AssetType, Wave, Stream, Unknown)
///
/// at namespace scope.
///
/// For the common case where enumerators do not require custom values, use SEAD_ENUM.
///
#define SEAD_ENUM_EX(NAME, ...)                                                                    \
    class NAME                                                                                     \
    {                                                                                              \
    public:                                                                                        \
        enum ValueType                                                                             \
        {                                                                                          \
            __VA_ARGS__                                                                            \
        };                                                                                         \
                                                                                                   \
        struct IndexTag                                                                            \
        {                                                                                          \
        };                                                                                         \
                                                                                                   \
        NAME() : mIdx(0) {}                                                                        \
        /* NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-pro-type-member-init) */   \
        NAME(ValueType value) : NAME(static_cast<int>(value)) {}                                   \
        /* NOLINTNEXTLINE(google-explicit-constructor,cppcoreguidelines-pro-type-member-init) */   \
        NAME(int value)                                                                            \
        {                                                                                          \
            int idx = findRelativeIndex_(static_cast<ValueType>(value));                           \
            if (idx == -1)                                                                         \
            {                                                                                      \
                SEAD_ASSERT_MSG(false, "could not convert to relative index: %d", idx);            \
                idx = 0;                                                                           \
            }                                                                                      \
            setRelativeIndex(idx);                                                                 \
        }                                                                                          \
        NAME(IndexTag, int index) : mIdx(index) {}                                                 \
                                                                                                   \
        NAME& operator=(const NAME& other) = default;                                              \
        bool operator==(const NAME& rhs) const { return mIdx == rhs.mIdx; }                        \
        bool operator!=(const NAME& rhs) const { return mIdx != rhs.mIdx; }                        \
        bool operator==(ValueType value) const { return mIdx == findRelativeIndex_(value); }       \
        bool operator!=(ValueType value) const { return mIdx != findRelativeIndex_(value); }       \
                                                                                                   \
        int value() const { return getArray_()[getRelativeIndex()]; }                              \
        int value() const volatile { return getArray_()[getRelativeIndex()]; }                     \
        /* XXX: Bafflingly, there is no purely const-qualified version of operator int().  */      \
        /* This leads to suboptimal codegen in many places. */                                     \
        operator int() const volatile { return value(); }                                          \
                                                                                                   \
        bool fromText(const sead::SafeString& name)                                                \
        {                                                                                          \
            for (auto it = begin(), it_end = end(); it != it_end; ++it)                            \
            {                                                                                      \
                if (name == it.get().text())                                                       \
                {                                                                                  \
                    setRelativeIndex(it.getIndex());                                               \
                    return true;                                                                   \
                }                                                                                  \
            }                                                                                      \
            return false;                                                                          \
        }                                                                                          \
        const char* text() const { return text(mIdx); }                                            \
        const char* text() const volatile { return text(mIdx); }                                   \
        static const char* text(int idx) { return text_(idx); }                                    \
                                                                                                   \
        int getRelativeIndex() const { return mIdx; }                                              \
        int getRelativeIndex() const volatile { return mIdx; }                                     \
        void setRelativeIndex(int idx)                                                             \
        {                                                                                          \
            SEAD_ASSERT_MSG(u32(idx) < size(), "range over: %d, [%d - %d)", idx, 0, size());       \
            mIdx = idx;                                                                            \
        }                                                                                          \
        void setRelativeIndex(int idx) volatile                                                    \
        {                                                                                          \
            SEAD_ASSERT_MSG(u32(idx) < size(), "range over: %d, [%d - %d)", idx, 0, size());       \
            mIdx = idx;                                                                            \
        }                                                                                          \
        static int findRelativeIndex(ValueType value) { return findRelativeIndex_(value); }        \
                                                                                                   \
        const char* getTypeText() const { return #NAME; }                                          \
        const char* getTypeText() const volatile { return #NAME; }                                 \
        constexpr static int size() { return cCount; }                                             \
        constexpr static int getSize() { return size(); }                                          \
        constexpr static int getLastIndex() { return size() - 1; }                                 \
                                                                                                   \
        static void initialize() { text(0); }                                                      \
                                                                                                   \
        class iterator                                                                             \
        {                                                                                          \
        public:                                                                                    \
            explicit iterator(int idx) : mIdx(idx) {}                                              \
            bool operator==(const iterator& rhs) const { return mIdx == rhs.mIdx; }                \
            bool operator!=(const iterator& rhs) const { return mIdx != rhs.mIdx; }                \
            iterator& operator++()                                                                 \
            {                                                                                      \
                if (mIdx > getLastIndex())                                                         \
                {                                                                                  \
                    SEAD_ASSERT_MSG(false, "enum iterator overflow");                              \
                    mIdx = getLastIndex();                                                         \
                }                                                                                  \
                else                                                                               \
                {                                                                                  \
                    ++mIdx;                                                                        \
                }                                                                                  \
                return *this;                                                                      \
            }                                                                                      \
            iterator& operator--()                                                                 \
            {                                                                                      \
                --mIdx;                                                                            \
                return *this;                                                                      \
            }                                                                                      \
            NAME operator*() const { return get(); }                                               \
            NAME get() const { return NAME(IndexTag{}, mIdx); }                                    \
            int getIndex() const { return mIdx; }                                                  \
                                                                                                   \
        private:                                                                                   \
            int mIdx;                                                                              \
        };                                                                                         \
                                                                                                   \
        static iterator begin() { return iterator(0); }                                            \
        static iterator end() { return iterator(getArray_().size()); }                             \
                                                                                                   \
    private:                                                                                       \
        class ValueArray                                                                           \
        {                                                                                          \
        public:                                                                                    \
            ValueArray();                                                                          \
                                                                                                   \
            s32 size() const { return mSize; }                                                     \
                                                                                                   \
            int& operator[](s32 idx) { return *get(idx); }                                         \
            const int& operator[](s32 idx) const { return *get(idx); }                             \
                                                                                                   \
            int* get(s32 idx)                                                                      \
            {                                                                                      \
                if (u32(mSize) <= u32(idx))                                                        \
                {                                                                                  \
                    SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", idx, mSize);                  \
                    return mBuffer;                                                                \
                }                                                                                  \
                return &mBuffer[idx];                                                              \
            }                                                                                      \
                                                                                                   \
            const int* get(s32 idx) const                                                          \
            {                                                                                      \
                if (u32(mSize) <= u32(idx))                                                        \
                {                                                                                  \
                    SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", idx, mSize);                  \
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
                                                                                                   \
        static ValueArray& getArray_()                                                             \
        {                                                                                          \
            static ValueArray sBuffer;                                                             \
            return sBuffer;                                                                        \
        }                                                                                          \
                                                                                                   \
        /* Returns nullptr when not found. */                                                      \
        static const char* text_(int idx)                                                          \
        {                                                                                          \
            if (u32(idx) >= cCount)                                                                \
                return nullptr;                                                                    \
                                                                                                   \
            static char** spTextPtr = nullptr;                                                     \
            if (spTextPtr)                                                                         \
                return spTextPtr[idx];                                                             \
            {                                                                                      \
                sead::ScopedLock<sead::CriticalSection> lock(sead::EnumUtil::getParseTextCS_());   \
                if (!spTextPtr)                                                                    \
                {                                                                                  \
                    static char* sTextPtr[cCount];                                                 \
                    static sead::FixedSafeString<cTextAllLen> sTextAll =                           \
                        sead::SafeString(cTextAll);                                                \
                    sead::EnumUtil::parseText_(sTextPtr, sTextAll.getBuffer(), cCount);            \
                    spTextPtr = sTextPtr;                                                          \
                }                                                                                  \
            }                                                                                      \
            return spTextPtr[idx];                                                                 \
        }                                                                                          \
                                                                                                   \
        /* Returns -1 when not found. */                                                           \
        static int findRelativeIndex_(ValueType value)                                             \
        {                                                                                          \
            const ValueArray& array = getArray_();                                                 \
            for (int i = 0, n = array.size(); i < n; ++i)                                          \
            {                                                                                      \
                if (array[i] == value)                                                             \
                    return i;                                                                      \
            }                                                                                      \
            return -1;                                                                             \
        }                                                                                          \
                                                                                                   \
        static constexpr const char* cTextAll = #__VA_ARGS__;                                      \
        static constexpr size_t cTextAllLen = sizeof(#__VA_ARGS__);                                \
        static constexpr int cCount = sead::EnumUtil::countValues(cTextAll, cTextAllLen);          \
                                                                                                   \
        int mIdx;                                                                                  \
    };

/// For use with SEAD_ENUM_EX.
#define SEAD_ENUM_EX_VALUES(NAME, ...)                                                             \
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
