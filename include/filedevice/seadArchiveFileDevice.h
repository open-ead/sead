#pragma once

#include "filedevice/seadFileDevice.h"

namespace sead
{
class ArchiveFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(ArchiveFileDevice, FileDevice)
public:
    using FileDevice::FileDevice;
    ~ArchiveFileDevice() override = default;

protected:
    bool doIsAvailable_() const override;
    u8* doLoad_(LoadArg& arg) override;
    FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag) override;
    bool doClose_(FileHandle* handle) override;
    bool doFlush_(FileHandle* handle) override;
    bool doRemove_(const SafeString& str) override;
    bool doRead_(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead) override;
    bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer,
                  u32 bytesToWrite) override;
    bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) override;
    bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle) override;
    bool doGetFileSize_(u32* fileSize, const SafeString& path) override;
    bool doGetFileSize_(u32* fileSize, FileHandle* handle) override;
    bool doIsExistFile_(bool* exists, const SafeString& path) override;
    bool doIsExistDirectory_(bool* exists, const SafeString& path) override;
    FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path) override;
    bool doCloseDirectory_(DirectoryHandle* handle) override;
    bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries,
                          u32 entriesToRead) override;
    bool doMakeDirectory_(const SafeString& path, u32 u_32) override;
    s32 doGetLastRawError_() const override;

    u8* doLoadWithEntryID_(s32 id, LoadArg& arg);
    FileDevice* doOpenWithEntryID_(FileHandle* handle, s32 id, FileOpenFlag flag);
    s32 doConvertPathToEntryID_(const SafeString& path);
    bool doSetCurrentDirectory_(const SafeString& path);
};
}  // namespace sead
