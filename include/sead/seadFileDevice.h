#ifndef SEAD_FILE_DEVICE_H_
#define SEAD_FILE_DEVICE_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>
#include <sead/seadRuntimeTypeInfo.h>

namespace sead {

class FileDevice;

class HandleBase : public IDisposer
{
public:
    HandleBase()
        : device(nullptr)
        , originalDevice(nullptr)
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
        if (_device != nullptr)
            _device->tryClose(this);
    }

    u32 tryRead(u8* outBuffer, u32 bytesToRead);

    u32 filesize;
};

class FileDevice : public UnkList, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    FileDevice()
        : UnkList()
        , IDisposer()
        , name()
        , _4C(1)
    {
    }

    virtual ~FileDevice()
    {
        if (FileDeviceMgr::sInstance != nullptr)
            FileDeviceMgr::sInstance->unmount(this);
    }

    // ...

    FileDevice* tryOpen(FileHandle*, const SafeString&, FileOpenFlag, u32);
    bool tryClose(FileHandle*);

    static const s32 cBufferMinAlignment = 0x40;

    FixedSafeString<32> name;
    u8 _4C;
};

} // namespace sead

#endif // SEAD_FILE_DEVICE_H_
