#include "stream/seadRamStream.h"

namespace sead
{
RamStreamSrc::RamStreamSrc(void* buffer, u32 bufferSize)
    : mBuffer(static_cast<u8*>(buffer)), mBufferSize(bufferSize)
{
}

RamStreamSrc::~RamStreamSrc() = default;

u32 RamStreamSrc::read(void* data, u32 size)
{
    if (mCurrentPos + size > mBufferSize)
    {
        size = mBufferSize - mCurrentPos;
    }

    memcpy(data, mBuffer + mCurrentPos, size);
    mCurrentPos += size;
    return size;
}

u32 RamStreamSrc::write(const void* data, u32 size)
{
    if (mCurrentPos + size > mBufferSize)
    {
        size = mBufferSize - mCurrentPos;
    }

    memcpy(mBuffer + mCurrentPos, data, size);
    mCurrentPos += size;
    return size;
}

u32 RamStreamSrc::skip(s32 offset)
{
    if (offset > 0 && mCurrentPos + offset > mBufferSize)
    {
        offset = mBufferSize - mCurrentPos;
    }

    if (offset < 0 && mCurrentPos < static_cast<u32>(-offset))
    {
        offset = -mCurrentPos;
    }

    mCurrentPos += offset;
    return offset;
}

void RamStreamSrc::rewind()
{
    mCurrentPos = 0;
}

bool RamStreamSrc::isEOF()
{
    return mCurrentPos >= mBufferSize;
}

RamReadStream::RamReadStream(const void* buffer, u32 buffer_size, Stream::Modes mode)
    : mSrc(const_cast<void*>(buffer), buffer_size)
{
    setSrc(&mSrc);
    setMode(mode);
}

RamReadStream::RamReadStream(const void* buffer, u32 buffer_size, StreamFormat* format)
    : mSrc(const_cast<void*>(buffer), buffer_size)
{
    setSrc(&mSrc);
    setUserFormat(format);
}

RamReadStream::~RamReadStream()
{
    setSrc(nullptr);
}

RamWriteStream::RamWriteStream(void* buffer, u32 buffer_size, Stream::Modes mode)
    : mSrc(buffer, buffer_size)
{
    setSrc(&mSrc);
    setMode(mode);
}

RamWriteStream::RamWriteStream(void* buffer, u32 buffer_size, StreamFormat* format)
    : mSrc(buffer, buffer_size)
{
    setSrc(&mSrc);
    setUserFormat(format);
}

RamWriteStream::~RamWriteStream()
{
    flush();
    setSrc(nullptr);
}

}  // namespace sead
