#ifndef SEAD_FILEDEVICE_H_
#define SEAD_FILEDEVICE_H_

#include <sead/seadDisposer.h>
#include <sead/seadHeap.h>
#include <sead/seadListImpl.h>
#include <sead/seadRuntimeTypeInfo.h>
#include <sead/seadSafeString.h>

namespace sead {

class FileDevice;

class HandleBase : public IDisposer
{
public:
    HandleBase()
        : IDisposer()
        , device(NULL)
        , originalDevice(NULL)
        , buffer()
    {
    }

    virtual ~HandleBase() { }

    FileDevice* device;
    FileDevice* originalDevice;
    u8 buffer[0x20];             // SafeArray<u8, 32>
};



class FileHandle;
class DirectoryHandle;
class DirectoryEntry;

class FileDevice : public UnkList, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    enum FileOpenFlag
    {
        FlagRead = 0,           // r
        FlagWriteTrunc = 1,     // w
        FlagReadWrite = 2,      // r+
        FlagReadWriteTrunc = 3  // w+
    };

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
    virtual s32 doGetLastRawError_() const = 0;
    virtual void doTracePath_(const SafeString& path) const;
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    FileDevice* tryOpen(FileHandle* handle, const SafeString& path, FileOpenFlag flag, u32 divSize);
    bool tryRead(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead);
    u8* tryLoad(LoadArg& arg);
    bool tryClose(FileHandle* handle);
    bool tryGetFileSize(u32* fileSize, FileHandle* handle);
    bool tryCloseDirectory(DirectoryHandle* handle);

    void setFileHandleDivSize_(FileHandle* handle, u32 divSize) const;
    void setHandleBaseFileDevice_(HandleBase* handle, FileDevice* device) const;
    void setHandleBaseOriginalFileDevice_(HandleBase* handle, FileDevice* device) const;

    inline u8* getHandleBaseHandleBuffer_(HandleBase* handle) const
    {
        return handle->buffer;
    }

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> mName;
    u8 _4C;
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

class DirectoryHandle : public HandleBase
{
public:
    DirectoryHandle()
        : HandleBase()
    {
    }

    virtual ~DirectoryHandle()
    {
        FileDevice* _device = originalDevice;
        if (_device != NULL)
            _device->tryCloseDirectory(this);
    }
};

class DirectoryEntry
{
public:
    DirectoryEntry()
        : mName()
        , isDirectory(false)
    {
    }

    ~DirectoryEntry() { }

    FixedSafeString<256> mName;
    bool isDirectory;
};

} // namespace sead

#endif // SEAD_FILEDEVICE_H_
