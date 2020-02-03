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

public:
    FileDevice()
        : UnkList()
        , IDisposer()
        , name()
        , _4C(1)
    {
    }

    virtual ~FileDevice();

    // ...

    FileDevice* tryOpen(FileHandle*, const SafeString&, FileOpenFlag, u32);
    bool tryClose(FileHandle*);

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> name;
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
    CafeFSAFileDevice();
    virtual ~CafeFSAFileDevice() { }

    u8 _54[24];
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
    MainFileDevice();
    virtual ~MainFileDevice();

    CafeContentFileDevice* mFileDevice;
};

} // namespace sead

#endif // SEAD_FILEDEVICE_H_
