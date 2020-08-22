#ifndef SEAD_MAIN_FILEDEVICE_H_
#define SEAD_MAIN_FILEDEVICE_H_

#ifdef cafe
#include <filedevice/cafe/seadCafeFSAFileDeviceCafe.h>
#endif  // cafe

#include <basis/seadTypes.h>
#include <filedevice/seadFileDevice.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

namespace sead
{
class MainFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(MainFileDevice, FileDevice)

public:
    MainFileDevice(Heap* heap);
    ~MainFileDevice() override;

    void traceFilePath(const SafeString& path) const override;
    void traceDirectoryPath(const SafeString& path) const override;
    void resolveFilePath(BufferedSafeString* out, const SafeString& path) const override;
    void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const override;
    bool isMatchDevice_(const HandleBase* handle) const override;
    bool doIsAvailable_() const override;
    FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag) override;
    bool doClose_(FileHandle* handle) override;
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
    bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle,
                                  DirectoryEntry* entries, u32 entriesToRead) override;
    bool doMakeDirectory_(const SafeString& path, u32) override;
    s32 doGetLastRawError_() const override;

#ifdef cafe
    CafeContentFileDevice* mFileDevice;
#else
#warning "Unknown platform"
#endif  // cafe
};

}  // namespace sead

#endif  // SEAD_MAIN_FILEDEVICE_H_
