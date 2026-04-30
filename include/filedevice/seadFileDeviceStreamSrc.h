#pragma once

#include <basis/seadTypes.h>
#include <filedevice/seadFileDevice.h>
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

private:
    FileHandle* mFileHandle = nullptr;
    u32 mStartingPos = 0;
    FileHandle _18;
    bool mIsHandleOpen = false;
    u32 mFileSize = 0;
};
}  // namespace sead
