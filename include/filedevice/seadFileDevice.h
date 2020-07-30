#ifndef SEAD_FILEDEVICE_H_
#define SEAD_FILEDEVICE_H_

#include <basis/seadTypes.h>
#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.hpp>

namespace sead
{
class FileDevice;

class HandleBase : public IDisposer
{
public:
    HandleBase() : IDisposer(), mDevice(NULL), mOriginalDevice(NULL), mHandleBuffer() {}

    virtual ~HandleBase() {}

    FileDevice* mDevice;
    FileDevice* mOriginalDevice;
    u8 mHandleBuffer[0x20];  // HandleBuffer = SafeArray<u8, 32>
};

class FileHandle;
class DirectoryHandle;
struct DirectoryEntry;

class FileDevice : public TListNode<FileDevice*>, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    enum FileOpenFlag
    {
        cFileOpenFlag_ReadOnly = 0,   // r
        cFileOpenFlag_WriteOnly = 1,  // w
        cFileOpenFlag_ReadWrite = 2,  // r+
        cFileOpenFlag_Create = 3      // w+
    };

    enum SeekOrigin
    {
        cSeekOrigin_Begin = 0,
        cSeekOrigin_Current = 1,
        cSeekOrigin_End = 2
    };

    struct LoadArg
    {
        LoadArg()
            : path(""), buffer(NULL), buffer_size(0), heap(NULL), alignment(0), div_size(0),
              read_size(0), roundup_size(0), need_unload(false)
        {
        }

        LoadArg(const LoadArg& arg)
            : path(arg.path), buffer(arg.buffer), buffer_size(arg.buffer_size), heap(arg.heap),
              alignment(arg.alignment), div_size(arg.div_size), read_size(arg.read_size),
              roundup_size(arg.roundup_size), need_unload(arg.need_unload)
        {
        }

        SafeString path;
        u8* buffer;
        u32 buffer_size;
        Heap* heap;
        s32 alignment;
        u32 div_size;
        u32 read_size;
        u32 roundup_size;
        bool need_unload;
    };

public:
    FileDevice() : TListNode<FileDevice*>(this), IDisposer(), mDriveName(), mPermission(true) {}

    FileDevice(const SafeString& name)
        : TListNode<FileDevice*>(this), IDisposer(), mDriveName(), mPermission(true)
    {
        mDriveName.copy(name);
    }

    virtual ~FileDevice();

    virtual void traceFilePath(const SafeString& path) const;
    virtual void traceDirectoryPath(const SafeString& path) const;
    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;
    virtual bool isMatchDevice_(const HandleBase* handle) const;
    virtual bool doIsAvailable_() const = 0;
    virtual u8* doLoad_(LoadArg& arg);
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag) = 0;
    virtual bool doClose_(FileHandle* handle) = 0;
    virtual bool doRead_(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead) = 0;
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer,
                          u32 bytesToWrite) = 0;
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) = 0;
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle) = 0;
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path) = 0;
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle) = 0;
    virtual bool doIsExistFile_(bool* exists, const SafeString& path) = 0;
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path) = 0;
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path) = 0;
    virtual bool doCloseDirectory_(DirectoryHandle* handle) = 0;
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle,
                                  DirectoryEntry* entries, u32 entriesToRead) = 0;
    virtual bool doMakeDirectory_(const SafeString& path, u32) = 0;
    virtual s32 doGetLastRawError_() const = 0;
    virtual void doTracePath_(const SafeString& path) const;
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    bool isAvailable() const;
    u8* tryLoad(LoadArg& arg);
    FileDevice* tryOpen(FileHandle* handle, const SafeString& path, FileOpenFlag flag, u32 divSize);
    bool tryClose(FileHandle* handle);
    bool tryRead(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    bool tryWrite(u32* bytesWritten, FileHandle* handle, const u8* inBuffer, u32 bytesToWrite);
    bool trySeek(FileHandle* handle, s32 offset, SeekOrigin origin);
    bool tryGetCurrentSeekPos(u32* seekPos, FileHandle* handle);
    bool tryGetFileSize(u32* fileSize, const SafeString& path);
    bool tryGetFileSize(u32* fileSize, FileHandle* handle);
    bool tryIsExistFile(bool* exists, const SafeString& path);
    bool tryIsExistDirectory(bool* exists, const SafeString& path);
    FileDevice* tryOpenDirectory(DirectoryHandle* handle, const SafeString& path);
    bool tryCloseDirectory(DirectoryHandle* handle);
    bool tryReadDirectory(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries,
                          u32 entriesToRead);
    bool tryMakeDirectory(const SafeString& path, u32);
    s32 getLastRawError() const;

    void setFileHandleDivSize_(FileHandle* handle, u32 divSize) const;
    void setHandleBaseFileDevice_(HandleBase* handle, FileDevice* device) const;
    void setHandleBaseOriginalFileDevice_(HandleBase* handle, FileDevice* device) const;

    inline u8* getHandleBaseHandleBuffer_(HandleBase* handle) const
    {
        return handle->mHandleBuffer;
    }

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> mDriveName;
    bool mPermission;
};

class FileHandle : public HandleBase
{
public:
    FileHandle() : HandleBase(), mDivSize(0) {}

    virtual ~FileHandle()
    {
        FileDevice* _device = mOriginalDevice;
        if (_device != NULL)
            _device->tryClose(this);
    }

    u32 read(u8* outBuffer, u32 bytesToRead);

    s32 mDivSize;
};

class DirectoryHandle : public HandleBase
{
public:
    DirectoryHandle() : HandleBase() {}

    virtual ~DirectoryHandle()
    {
        FileDevice* _device = mOriginalDevice;
        if (_device != NULL)
            _device->tryCloseDirectory(this);
    }
};

struct DirectoryEntry
{
    DirectoryEntry() : name(), is_directory(false) {}

    FixedSafeString<256> name;
    bool is_directory;
};

}  // namespace sead

#endif  // SEAD_FILEDEVICE_H_
