#pragma once

#include <basis/seadTypes.h>
#include <filedevice/seadFileDevice.h>
#include <stream/seadStream.h>
#include <stream/seadStreamSrc.h>

namespace sead
{
class FileDeviceStreamSrc : public StreamSrc
{
public:
    FileDeviceStreamSrc();
    FileDeviceStreamSrc(FileHandle* fileHandle);
    ~FileDeviceStreamSrc() override;

    u32 read(void* buffer, u32 size) override;
    u32 write(const void* buffer, u32 size) override;
    u32 skip(s32 offset) override;
    void rewind() override;
    bool isEOF() override;

    void setFileHandle(sead::FileHandle* fileHandle);

    FileHandle* getFileHandle() const { return mFileHandle; }

private:
    FileHandle* mFileHandle = nullptr;
    u32 mStartingPos = 0;
    FileHandle _18;
    bool mIsHandleOpen = false;
    u32 mFileSize = 0;
};

class FileDeviceWriteStream : public WriteStream
{
    FileDeviceWriteStream(Stream::Modes mode);
    FileDeviceWriteStream(StreamFormat* format);
    FileDeviceWriteStream(FileHandle* fileHandle, Stream::Modes mode);
    FileDeviceWriteStream(FileHandle* fileHandle, StreamFormat* format);
    ~FileDeviceWriteStream() override;

    void setFileHandle(sead::FileHandle* fileHandle);

private:
    FileDeviceStreamSrc src;
};

class FileDeviceReadStream : public ReadStream
{
    FileDeviceReadStream(Stream::Modes mode);
    FileDeviceReadStream(StreamFormat* format);
    FileDeviceReadStream(FileHandle* fileHandle, Stream::Modes mode);
    FileDeviceReadStream(FileHandle* fileHandle, StreamFormat* format);
    ~FileDeviceReadStream() override;

    void setFileHandle(sead::FileHandle* fileHandle);

private:
    FileDeviceStreamSrc src;
};

}  // namespace sead
