#ifndef SEAD_FILEDEVICE_H_
#define SEAD_FILEDEVICE_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>
#include <sead/seadRuntimeTypeInfo.h>

namespace sead {

class FileHandle;

class FileDevice : public UnkList, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    enum FileOpenFlag { };

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
            , isValid(false)
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
            , isValid(arg.isValid)
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
        bool isValid;
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
    // ...

    FileDevice* tryOpen(FileHandle* handle, const SafeString& path, FileOpenFlag flag, u32 divSize);
    u8* tryLoad(LoadArg& arg);
    bool tryClose(FileHandle*);

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> mName;
    u8 _4C;
};

class HandleBase : public IDisposer
{
public:
    HandleBase()
        : device(NULL)
        , originalDevice(NULL)
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
        , filesize(0)
    {
    }

    virtual ~FileHandle()
    {
        FileDevice* _device = originalDevice;
        if (_device != NULL)
            _device->tryClose(this);
    }

    u32 tryRead(u8* outBuffer, u32 bytesToRead);

    u32 filesize;
};

class CafeFSAFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(CafeFSAFileDevice, FileDevice)

public:
    CafeFSAFileDevice(const SafeString& name, const SafeString& devicePath);
    virtual ~CafeFSAFileDevice() { }

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

    CafeContentFileDevice* mFileDevice;
};

} // namespace sead

#endif // SEAD_FILEDEVICE_H_
