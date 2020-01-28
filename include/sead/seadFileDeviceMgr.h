#ifndef SEAD_FILEDEVICEMGR_H_
#define SEAD_FILEDEVICEMGR_H_

#include <coreinit/fs.h>
#include <sead/seadFileDevice.h>
#include <sead/seadListImpl.h>
#include <sead/seadSafeString.h>

namespace sead {

class FileDeviceMgr
{
public:
    FileDeviceMgr();

    FileDevice* tryOpen(FileHandle*, const SafeString&, FileDevice::FileOpenFlag, u32);

    static FileDeviceMgr* sInstance;

    IDisposer staticDisposer;
    ListImpl mountedDevices;
    MainFileDevice* device;
    FSClient client;
    u8 _1724[128];
    u8 _17A4[128];
    u32 _1824;
    void* vtable;
};

} // namespace sead

#endif // SEAD_FILEDEVICEMGR_H_
