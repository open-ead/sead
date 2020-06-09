#ifndef SEAD_PTR_ARRAY_H_
#define SEAD_PTR_ARRAY_H_

#include <sead/seadSystem.h>
#include <sead/types.h>

namespace sead
{
class PtrArrayImpl
{
public:
    __attribute__((always_inline)) PtrArrayImpl(s32 ptrNumMax, void* buf)
        : mPtrNum(0), mPtrNumMax(0), mPtrs(NULL)
    {
        setBuffer(ptrNumMax, buf);
    }

    void setBuffer(s32 ptrNumMax, void* buf);

    s32 size() const { return mPtrNum; }
    s32 capacity() const { return mPtrNumMax; }

protected:
    s32 mPtrNum;
    s32 mPtrNumMax;
    void** mPtrs;
};

template <typename T>
class PtrArray : public PtrArrayImpl
{
public:
    PtrArray(s32 ptrNumMax, T** buf) : PtrArrayImpl(ptrNumMax, buf) {}

    class iterator
    {
    public:
        iterator(T** pptr) : mPPtr{pptr} {}
        bool operator==(const iterator& other) const { return mPPtr == other.mPPtr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        iterator& operator++()
        {
            ++mPPtr;
            return *this;
        }
        iterator operator++(int)
        {
            iterator old = *this;
            ++mPPtr;
            return old;
        }
        T*& operator*() const { return *mPPtr; }
        T** operator->() const { return mPPtr; }

    private:
        T** mPPtr;
    };

    class constIterator
    {
    public:
        constIterator(const T** pptr) : mPPtr{pptr} {}
        bool operator==(const constIterator& other) const { return mPPtr == other.mPPtr; }
        bool operator!=(const constIterator& other) const { return !(*this == other); }
        constIterator& operator++()
        {
            ++mPPtr;
            return *this;
        }
        constIterator operator++(int)
        {
            constIterator old = *this;
            ++mPPtr;
            return old;
        }
        T* operator*() const { return *mPPtr; }
        const T** operator->() const { return mPPtr; }

    private:
        const T** mPPtr;
    };

    iterator begin() const { return iterator(data()); }
    iterator end() const { return iterator(data() + mPtrNum); }
    constIterator cbegin() const { return constIterator(data()); }
    constIterator cend() const { return constIterator(data() + mPtrNum); }

    T** data() const { return reinterpret_cast<T**>(mPtrs); }

    T*& operator[](size_t idx)
    {
        SEAD_ASSERT(idx < size_t(mPtrNum), "index exceeded [%d/%d]", int(idx), mPtrNum);
        return idx < size_t(mPtrNum) ? data()[idx] : data()[0];
    }

    T* operator[](size_t idx) const
    {
        SEAD_ASSERT(idx < size_t(mPtrNum), "index exceeded [%d/%d]", int(idx), mPtrNum);
        return idx < size_t(mPtrNum) ? data()[idx] : data()[0];
    }
};

template <typename T, s32 N>
class FixedPtrArray : public PtrArray<T>
{
public:
    FixedPtrArray() : PtrArray<T>(N, mWork) {}

private:
    // Nintendo uses an untyped u8[N*sizeof(void*)] buffer. That is undefined behavior,
    // so we will not do that.
    T* mWork[N];
};

}  // namespace sead

#endif  // SEAD_PTR_ARRAY_H_
