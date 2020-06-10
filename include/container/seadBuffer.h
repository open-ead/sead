#ifndef SEAD_BUFFER_H_
#define SEAD_BUFFER_H_

#include <basis/seadTypes.h>

namespace sead
{
template <typename T>
class Buffer
{
public:
    Buffer() : mSize(0), mBuffer(NULL) {}

    void setBuffer(s32 size, T* bufferptr)
    {
        if (size > 0 && bufferptr)
        {
            mSize = size;
            mBuffer = bufferptr;
        }
    }

    T* unsafeGet(s32 index) { return &mBuffer[index]; }

    const T* unsafeGet(s32 index) const { return &mBuffer[index]; }

    T& operator[](s32 index) { return *unsafeGet(index); }

    const T& operator[](s32 index) const { return *unsafeGet(index); }

    s32 mSize;
    T* mBuffer;
};

}  // namespace sead

#endif  // SEAD_BUFFER_H_
