#include <stream/seadFileDeviceStream.h>

namespace sead
{
FileDeviceStreamSrc::FileDeviceStreamSrc() = default;

FileDeviceStreamSrc::FileDeviceStreamSrc(FileHandle* fileHandle)
    : mFileHandle(fileHandle), mStartingPos(fileHandle->getCurrentSeekPos()),
      mFileSize(fileHandle->getFileSize())
{
}

FileDeviceStreamSrc::~FileDeviceStreamSrc()
{
    if (mIsHandleOpen && mFileHandle)
        mFileHandle->close();
}

u32 FileDeviceStreamSrc::read(void* buffer, u32 size)
{
    return mFileHandle->read(static_cast<u8*>(buffer), size);
}

u32 FileDeviceStreamSrc::write(const void* buffer, u32 size)
{
    return mFileHandle->write(static_cast<const u8*>(buffer), size);
}

u32 FileDeviceStreamSrc::skip(s32 offset)
{
    if (!mFileHandle->seek(offset, FileDevice::cSeekOrigin_Current))
        return 0;
    return offset;
}

void FileDeviceStreamSrc::rewind()
{
    mFileHandle->seek(mStartingPos, FileDevice::cSeekOrigin_Begin);
}

bool FileDeviceStreamSrc::isEOF()
{
    return mFileHandle->getCurrentSeekPos() >= mFileSize;
}

void FileDeviceStreamSrc::setFileHandle(sead::FileHandle* fileHandle)
{
    mFileHandle = fileHandle;
    if (fileHandle)
    {
        mStartingPos = fileHandle->getCurrentSeekPos();
        mFileSize = fileHandle->getFileSize();
    }
}

FileDeviceWriteStream::FileDeviceWriteStream(Stream::Modes mode)
{
    setSrc(&src);
    setMode(mode);
}

FileDeviceWriteStream::FileDeviceWriteStream(StreamFormat* format)
{
    setSrc(&src);
    setUserFormat(format);
}

FileDeviceWriteStream::FileDeviceWriteStream(FileHandle* fileHandle, Stream::Modes mode)
    : src(fileHandle)
{
    setSrc(&src);
    setMode(mode);
}

FileDeviceWriteStream::FileDeviceWriteStream(FileHandle* fileHandle, StreamFormat* format)
    : src(fileHandle)
{
    setSrc(&src);
    setUserFormat(format);
}

FileDeviceWriteStream::~FileDeviceWriteStream()
{
    flush();
    mSrc = nullptr;
}

void FileDeviceWriteStream::setFileHandle(sead::FileHandle* fileHandle)
{
    if (src.getFileHandle())
    {
        flush();
        rewind();
    }
    src.setFileHandle(fileHandle);
}

FileDeviceReadStream::FileDeviceReadStream(Stream::Modes mode)
{
    setSrc(&src);
    setMode(mode);
}

FileDeviceReadStream::FileDeviceReadStream(StreamFormat* format)
{
    setSrc(&src);
    setUserFormat(format);
}

FileDeviceReadStream::FileDeviceReadStream(FileHandle* fileHandle, Stream::Modes mode)
    : src(fileHandle)
{
    setSrc(&src);
    setMode(mode);
}

FileDeviceReadStream::FileDeviceReadStream(FileHandle* fileHandle, StreamFormat* format)
    : src(fileHandle)
{
    setSrc(&src);
    setUserFormat(format);
}

FileDeviceReadStream::~FileDeviceReadStream()
{
    mSrc = nullptr;
}

void FileDeviceReadStream::setFileHandle(sead::FileHandle* fileHandle)
{
    if (src.getFileHandle())
        rewind();

    src.setFileHandle(fileHandle);
}

BufferFileDeviceWriteStream::BufferFileDeviceWriteStream(Stream::Modes mode)
    : FileDeviceWriteStream(mode), mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceWriteStream::BufferFileDeviceWriteStream(StreamFormat* format)
    : FileDeviceWriteStream(format), mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceWriteStream::BufferFileDeviceWriteStream(FileHandle* fileHandle, Stream::Modes mode)
    : FileDeviceWriteStream(fileHandle, mode),
      mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceWriteStream::BufferFileDeviceWriteStream(FileHandle* fileHandle,
                                                         StreamFormat* format)
    : FileDeviceWriteStream(fileHandle, format),
      mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceReadStream::BufferFileDeviceReadStream(Stream::Modes mode)
    : FileDeviceReadStream(mode), mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceReadStream::BufferFileDeviceReadStream(StreamFormat* format)
    : FileDeviceReadStream(format), mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceReadStream::BufferFileDeviceReadStream(FileHandle* fileHandle, Stream::Modes mode)
    : FileDeviceReadStream(fileHandle, mode),
      mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

BufferFileDeviceReadStream::BufferFileDeviceReadStream(FileHandle* fileHandle, StreamFormat* format)
    : FileDeviceReadStream(fileHandle, format),
      mBufferSrc(getSrc(), PtrUtil::align(mBuffer, 0x20), 0x100)
{
    setSrc(&mBufferSrc);
}

}  // namespace sead
