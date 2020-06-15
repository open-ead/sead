#ifdef cafe
#include <cafe.h>
#include <nn/save.h>
#endif  // cafe

#include <basis/seadNew.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <filedevice/seadPath.h>
#include <heap/seadHeapMgr.h>

namespace sead
{
SEAD_SINGLETON_DISPOSER_IMPL(FileDeviceMgr)

FileDeviceMgr::FileDeviceMgr() : mDeviceList(), mMainFileDevice(NULL), mDefaultFileDevice(NULL)
{
    if (HeapMgr::sInstancePtr == NULL)
        return;

    Heap* containHeap = HeapMgr::sInstancePtr->findContainHeap(this);

#ifdef cafe
    FSInit();
    FSAddClient(&client, FS_RET_NO_ERROR);

    FSStateChangeParams changeParams = {
        .userCallback = stateChangeCallback_, .userContext = NULL, .ioMsgQueue = NULL};

    FSSetStateChangeNotification(&client, &changeParams);
    SAVEInit();
    _17A4[0] = 0;
    _1824 = 0;
#else
#error "Unknown platform"
#endif  // cafe

    mMainFileDevice = new (containHeap, 4) MainFileDevice(containHeap);
    mount(mMainFileDevice);

    mDefaultFileDevice = mMainFileDevice;
}

FileDeviceMgr::~FileDeviceMgr()
{
    if (mMainFileDevice != NULL)
    {
        delete mMainFileDevice;
        mMainFileDevice = NULL;
    }

#ifdef cafe
    FSDelClient(&client, FS_RET_NO_ERROR);
    SAVEShutdown();
    FSShutdown();
#else
#error "Unknown platform"
#endif  // cafe
}

void FileDeviceMgr::traceFilePath(const SafeString& path) const
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(path, &pathNoDrive);

    if (device != NULL)
        device->traceFilePath(pathNoDrive);
}

void FileDeviceMgr::traceDirectoryPath(const SafeString& path) const
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(path, &pathNoDrive);

    if (device != NULL)
        device->traceDirectoryPath(pathNoDrive);
}

void FileDeviceMgr::resolveFilePath(BufferedSafeString* out, const SafeString& path) const
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(path, &pathNoDrive);

    if (device != NULL)
        device->resolveFilePath(out, pathNoDrive);
}

void FileDeviceMgr::resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(path, &pathNoDrive);

    if (device != NULL)
        device->resolveDirectoryPath(out, pathNoDrive);
}

void FileDeviceMgr::mount(FileDevice* device, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        device->mDriveName.copy(name);

    mDeviceList.pushBack(device);
}

void FileDeviceMgr::unmount(FileDevice* device)
{
    if (device->mList != NULL)
        mDeviceList.erase(device);

    if (device == mDefaultFileDevice)
        mDefaultFileDevice = NULL;
}

FileDevice* FileDeviceMgr::findDeviceFromPath(const SafeString& path,
                                              BufferedSafeString* pathNoDrive) const
{
    FixedSafeString<32> driveName;
    FileDevice* device;

    if (!Path::getDriveName(&driveName, path))
    {
        device = mDefaultFileDevice;
        if (device == NULL)
            return NULL;
    }

    else
        device = findDevice(driveName);

    if (pathNoDrive != NULL)
        Path::getPathExceptDrive(pathNoDrive, path);

    return device;
}

FileDevice* FileDeviceMgr::findDevice(const SafeString& name) const
{
    for (TListNode<FileDevice*>* node = mDeviceList.root(); !mDeviceList.isAtEnd(node);
         node = mDeviceList.next(node))
        if (node->mData->mDriveName.isEqual(name))
            return node->mData;

    return NULL;
}

FileDevice* FileDeviceMgr::tryOpen(FileHandle* handle, const SafeString& path,
                                   FileDevice::FileOpenFlag flag, u32 divSize)
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(path, &pathNoDrive);

    if (device == NULL)
        return NULL;

    return device->tryOpen(handle, pathNoDrive, flag, divSize);
}

u8* FileDeviceMgr::tryLoad(FileDevice::LoadArg& arg)
{
    FixedSafeString<256> pathNoDrive;
    FileDevice* device = findDeviceFromPath(arg.path, &pathNoDrive);

    if (device == NULL)
        return NULL;

    FileDevice::LoadArg arg2(arg);
    arg2.path = pathNoDrive.cstr();

    u8* data = device->tryLoad(arg2);

    arg.read_size = arg2.read_size;
    arg.roundup_size = arg2.roundup_size;
    arg.need_unload = arg2.need_unload;

    return data;
}

#ifdef cafe
void FileDeviceMgr::stateChangeCallback_(FSClient* client, FSVolumeState state, void* context)
{
    FSGetLastError(client);
}
#endif  // cafe

}  // namespace sead
