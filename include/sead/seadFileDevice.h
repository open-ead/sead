#ifndef SEAD_FILEDEVICE_H_
#define SEAD_FILEDEVICE_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>
#include <sead/seadRuntimeTypeInfo.h>

namespace sead {

class HandleBase;
class FileHandle;
class DirectoryHandle;
class DirectoryEntry;

class FileDevice : public UnkList, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    enum FileOpenFlag { };

    enum SeekOrigin
    {
        OriginBegin = 0,
        OriginCurrent = 1,
        OriginEnd = 2
    };

    struct LoadArg
    {
        LoadArg()
            : name("")
            , buffer(NULL)
            , bufferSize(0)
            , heap(NULL)
            , bufferSizeAlignment(0)
            , divSize(0)
            , fileSize(0)
            , allocSize(0)
            , allocated(false)
        {
        }

        LoadArg(const LoadArg& arg)
            : name(arg.name)
            , buffer(arg.buffer)
            , bufferSize(arg.bufferSize)
            , heap(arg.heap)
            , bufferSizeAlignment(arg.bufferSizeAlignment)
            , divSize(arg.divSize)
            , fileSize(arg.fileSize)
            , allocSize(arg.allocSize)
            , allocated(arg.allocated)
        {
        }

        SafeString name;
        u8* buffer;
        u32 bufferSize;
        Heap* heap;
        s32 bufferSizeAlignment;
        u32 divSize;
        u32 fileSize;
        u32 allocSize;
        bool allocated;
    };

public:
    FileDevice()
        : UnkList()
        , IDisposer()
        , mName()
        , _4C(1)
    {
    }

    FileDevice(const SafeString& name)
        : UnkList()
        , IDisposer()
        , mName()
        , _4C(1)
    {
        mName.copy(name);
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
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer, u32 bytesToWrite) = 0;
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) = 0;
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle) = 0;
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path) = 0;
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle) = 0;
    virtual bool doIsExistFile_(bool* exists, const SafeString& path) = 0;
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path) = 0;
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path) = 0;
    virtual bool doCloseDirectory_(DirectoryHandle* handle) = 0;
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries, u32 entriesToRead) = 0;
    virtual bool doMakeDirectory_(const SafeString& path, u32) = 0;
    virtual u32 doGetLastRawError_() const = 0;
    virtual void doTracePath_(const SafeString& path) const;
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    FileDevice* tryOpen(FileHandle* handle, const SafeString& path, FileOpenFlag flag, u32 divSize);
    bool tryRead(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    u8* tryLoad(LoadArg& arg);
    bool tryClose(FileHandle* handle);
    bool tryGetFileSize(u32* fileSize, FileHandle* handle);

    void setFileHandleDivSize_(FileHandle* handle, u32 divSize) const;
    void setHandleBaseFileDevice_(HandleBase* handle, FileDevice* device) const;
    void setHandleBaseOriginalFileDevice_(HandleBase* handle, FileDevice* device) const;

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> mName;
    u8 _4C;
};

class HandleBase : public IDisposer
{
public:
    HandleBase()
        : IDisposer()
        , device(NULL)
        , originalDevice(NULL)
        , _18()
    {
    }

    virtual ~HandleBase() { }

    FileDevice* device;
    FileDevice* originalDevice;
    u8 _18[0x20];
};

class FileHandle : public HandleBase
{
public:
    FileHandle()
        : HandleBase()
        , divSize(0)
    {
    }

    virtual ~FileHandle()
    {
        FileDevice* _device = originalDevice;
        if (_device != NULL)
            _device->tryClose(this);
    }

    u32 read(u8* outBuffer, u32 bytesToRead);

    s32 divSize;
};

class CafeFSAFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(CafeFSAFileDevice, FileDevice)

public:
    CafeFSAFileDevice(const SafeString& name, const SafeString& devicePath);
    virtual ~CafeFSAFileDevice() { }

    virtual bool doIsAvailable_() const;
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& path, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer, u32 bytesToWrite);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path);
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle);
    virtual bool doIsExistFile_(bool* exists, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries, u32 entriesToRead);
    virtual bool doMakeDirectory_(const SafeString& path, u32);
    virtual u32 doGetLastRawError_() const;
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    const char* devicePath;
    u32 _58;
    u32 _5C;
    u32 _60;
    u32 _64;
    u32 _68;
};

class CafeContentFileDevice : public CafeFSAFileDevice
{
    SEAD_RTTI_OVERRIDE(CafeContentFileDevice, CafeFSAFileDevice)

public:
    CafeContentFileDevice();
    virtual ~CafeContentFileDevice() { }
};

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
    virtual bool doWrite_(u32* bytesWritten, FileHandle* handle, const u8* inBuffer, u32 bytesToWrite);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* seekPos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* fileSize, const SafeString& path);
    virtual bool doGetFileSize_(u32* fileSize, FileHandle* handle);
    virtual bool doIsExistFile_(bool* exists, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* exists, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& path);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* entriesRead, DirectoryHandle* handle, DirectoryEntry* entries, u32 entriesToRead);
    virtual bool doMakeDirectory_(const SafeString& path, u32);
    virtual u32 doGetLastRawError_() const;

    CafeContentFileDevice* mFileDevice;
};

} // namespace sead

#endif // SEAD_FILEDEVICE_H_
