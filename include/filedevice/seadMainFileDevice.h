#ifndef SEAD_MAIN_FILEDEVICE_H_
#define SEAD_MAIN_FILEDEVICE_H_

#ifdef cafe
#include <filedevice/cafe/seadCafeFSAFileDeviceCafe.h>
#endif  // cafe

#include <basis/seadTypes.h>
#include <filedevice/seadFileDevice.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.hpp>

namespace sead
{
class MainFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(MainFileDevice, FileDevice)

public:
    MainFileDevice(Heap* heap);
    virtual ~MainFileDevice();

    virtual void traceFilePath(const SafeString& path) const;
    virtual void traceDirectoryPath(const SafeString& path) const;
    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;
    virtual bool isMatchDevice_(const HandleBase* handle) const;
    virtual bool doIsAvailable_() const;
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer,
                          u32 bytesToWrite);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path);
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle);
    virtual bool doIsExistFile_(bool* exists, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle,
                                  DirectoryEntry* entries, u32 entriesToRead);
    virtual bool doMakeDirectory_(const SafeString& path, u32);
    virtual s32 doGetLastRawError_() const;

#ifdef cafe
    CafeContentFileDevice* mFileDevice;
#else
#error "Unknown platform"
#endif  // cafe
};

}  // namespace sead

#endif  // SEAD_MAIN_FILEDEVICE_H_
