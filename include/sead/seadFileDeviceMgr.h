#ifndef SEAD_FILEDEVICEMGR_H_
#define SEAD_FILEDEVICEMGR_H_

#ifdef cafe
#include <cafe.h>
#endif  // cafe

#include <sead/seadDisposer.h>
#include <sead/seadFileDevice.h>
#include <sead/seadMainFileDevice.h>
#include <sead/seadSafeString.hpp>
#include <sead/seadTList.h>
#include <sead/types.h>

namespace sead
{
class FileDeviceMgr
{
    SEAD_SINGLETON_DISPOSER(FileDeviceMgr)

public:
    FileDeviceMgr();
    ~FileDeviceMgr();

    void traceFilePath(const SafeString& path) const;
    void traceDirectoryPath(const SafeString& path) const;
    void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;

    void mount(FileDevice* device, const SafeString& name = SafeString::cEmptyString);
    void unmount(FileDevice* device);
    FileDevice* findDeviceFromPath(const SafeString& path, BufferedSafeString* pathNoDrive) const;
    FileDevice* findDevice(const SafeString& name) const;

    FileDevice* tryOpen(FileHandle* handle, const SafeString& path, FileDevice::FileOpenFlag flag,
                        u32 divSize);
    u8* tryLoad(FileDevice::LoadArg& arg);

    typedef TList<FileDevice> DeviceList;

    DeviceList mDeviceList;
    FileDevice* mDefaultFileDevice;
    MainFileDevice* mMainFileDevice;

#ifdef cafe
    static void stateChangeCallback_(FSClient* client, FSVolumeState state, void* context);

    FSClient client;
    u8 _1724[128];
    u8 _17A4[128];
    u32 _1824;
#else
#error "Unknown platform"
#endif  // cafe
};

}  // namespace sead

#endif  // SEAD_FILEDEVICEMGR_H_
