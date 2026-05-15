#include "stream/seadBufferStream.h"

#include "math/seadMathCalcCommon.h"

namespace sead
{
BufferReadStreamSrc::BufferReadStreamSrc(StreamSrc* src, void* buffer, u32 buffer_size)
    : mSrc(src), mBuffer(buffer), mBufferSize(buffer_size)
{
}

BufferReadStreamSrc::~BufferReadStreamSrc() = default;

// NOTE: cannot take negative `offset`, but expects `mSrc->skip(X)` to work with negatives
u32 BufferReadStreamSrc::read(void* data, u32 size)
{
    u32 totalBytesRead = 0;
    while (true)
    {
        if (mCurrentPos < mCurrentSize)
        {
            u32 readSize = sead::Mathu::clampMax(size - totalBytesRead, mCurrentSize - mCurrentPos);

            memcpy((u8*)data + totalBytesRead, (u8*)mBuffer + mCurrentPos, readSize);
            totalBytesRead += readSize;
            mCurrentPos += readSize;
        }

        if (size <= totalBytesRead)
            break;

        mCurrentSize = mSrc->read(mBuffer, mBufferSize);
        mCurrentPos = 0;

        if (mCurrentSize == 0)
            break;
    }
    return totalBytesRead;
}

u32 BufferReadStreamSrc::write([[maybe_unused]] const void* data, [[maybe_unused]] u32 size)
{
    return 0;
}

u32 BufferReadStreamSrc::skip(s32 offset)
{
    s32 remainingBytes = mCurrentSize - mCurrentPos;

    if (remainingBytes >= offset)
    {
        mCurrentPos += offset;
        return offset;
    }

    mCurrentSize = 0;
    mCurrentPos = 0;
    return mSrc->skip(offset - remainingBytes) + remainingBytes;
}

void BufferReadStreamSrc::rewind()
{
    mSrc->rewind();
    mCurrentSize = 0;
    mCurrentPos = 0;
}

bool BufferReadStreamSrc::isEOF()
{
    return mSrc->isEOF() && mCurrentPos >= mCurrentSize;
}

BufferReadStream::BufferReadStream(ReadStream* stream, const void* buffer, u32 buffer_size)
    : mSrc(stream->getSrc(), const_cast<void*>(buffer), buffer_size)
{
    setSrc(&mSrc);
    setUserFormat(stream->getUserFormat());
    setBinaryEndian(stream->getBinaryEndian());
}

BufferReadStream::~BufferReadStream()
{
    setSrc(nullptr);
}

BufferWriteStreamSrc::BufferWriteStreamSrc(StreamSrc* src, void* buffer, u32 buffer_size)
    : mSrc(src), mBuffer(buffer), mBufferSize(buffer_size)
{
}

BufferWriteStreamSrc::~BufferWriteStreamSrc() = default;

u32 BufferWriteStreamSrc::read([[maybe_unused]] void* data, [[maybe_unused]] u32 size)
{
    return 0;
}

u32 BufferWriteStreamSrc::write(const void* data, u32 size)
{
    u32 totalBytesWritten = 0;
    do
    {
        if (mCurrentPos >= mBufferSize)
            continue;

        u32 writeSize = sead::Mathu::min(mBufferSize - mCurrentPos, size - totalBytesWritten);

        memcpy((u8*)mBuffer + mCurrentPos, (u8*)data + totalBytesWritten, writeSize);
        totalBytesWritten += writeSize;
        mCurrentPos += writeSize;
    } while (totalBytesWritten < size && flush());

    return totalBytesWritten;
}

u32 BufferWriteStreamSrc::skip([[maybe_unused]] s32 offset)
{
    return 0;
}

void BufferWriteStreamSrc::rewind()
{
    flush();
    mSrc->rewind();
}

bool BufferWriteStreamSrc::flush()
{
    if (mCurrentPos == 0)
        return true;

    bool isDone = mSrc->write(mBuffer, mCurrentPos) >= mCurrentPos;
    mCurrentPos = 0;
    return isDone;
}

BufferWriteStream::BufferWriteStream(WriteStream* stream, void* buffer, u32 buffer_size)
    : mSrc(stream->getSrc(), buffer, buffer_size)
{
    setSrc(&mSrc);
    setUserFormat(stream->getUserFormat());
    setBinaryEndian(stream->getBinaryEndian());
}

BufferWriteStream::~BufferWriteStream()
{
    flush();
    setSrc(nullptr);
}

}  // namespace sead
