#include <filedevice/seadFileDeviceStreamSrc.h>

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

}  // namespace sead
