#include <cafe.h>
#include <nn/save.h>

#include <sead.h>

namespace sead {

FileDeviceMgr* FileDeviceMgr::sInstance = NULL;
FileDeviceMgr::SingletonDisposer_* FileDeviceMgr::SingletonDisposer_::sStaticDisposer = NULL;

FileDeviceMgr::FileDeviceMgr()
    : mountedDevices()
    , mainDevice(NULL)
    , defaultDevice(NULL)
{
    if (HeapMgr::sInstancePtr == NULL)
        return;

    Heap* containHeap = HeapMgr::sInstancePtr->findContainHeap(this);

    FSInit();
    FSAddClient(&client, FS_RET_NO_ERROR);

    FSStateChangeParams changeParams = {
        .userCallback = stateChangeCallback_,
        .userContext  = NULL,
        .ioMsgQueue   = NULL
    };

    FSSetStateChangeNotification(&client, &changeParams);
    SAVEInit();
    _17A4[0] = 0;
    _1824 = 0;

    mainDevice = new(containHeap, 4) MainFileDevice(containHeap);
    mount(mainDevice);

    defaultDevice = mainDevice;
}

FileDeviceMgr::~FileDeviceMgr()
{
    if (mainDevice != NULL)
    {
        delete mainDevice;
        mainDevice = NULL;
    }

    FSDelClient(&client, FS_RET_NO_ERROR);
    SAVEShutdown();
    FSShutdown();
}

SEAD_CREATE_SINGLETON_INSTANCE(FileDeviceMgr, FileDeviceMgr::sInstance)
SEAD_DELETE_SINGLETON_INSTANCE(FileDeviceMgr, FileDeviceMgr::sInstance)

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
        device->mName.copy(name);

    ListImpl* parent = device->parent;
    if (parent != NULL)
    {
        device->parent = NULL;
        device->root.erase_();
        parent->mCount -= 1;
    }

    mountedDevices.setParentFor(device);
    mountedDevices.insertFront(device);
    mountedDevices.getCountRef() += 1;
}

void FileDeviceMgr::unmount(FileDevice* device)
{
    if (device->parent != NULL)
    {
        device->parent = NULL;
        device->root.erase_();
        mountedDevices.getCountRef() -= 1;
    }

    if (device == defaultDevice)
        defaultDevice = NULL;
}

FileDevice*
FileDeviceMgr::findDeviceFromPath(
    const SafeString& path, BufferedSafeString* pathNoDrive
) const
{
    FixedSafeString<32> driveName;
    FileDevice* device;

    if(!Path::getDriveName(&driveName, path))
    {
        device = defaultDevice;
        if (device == NULL)
            return NULL;
    }

    else
        device = findDevice(driveName);

    if (pathNoDrive != NULL)
        Path::getPathExceptDrive(pathNoDrive, path);

    return device;
}

FileDevice*
FileDeviceMgr::findDevice(const SafeString& name) const
{
    for (UnkList* list = mountedDevices.root(); !mountedDevices.isAtEnd(list); list = mountedDevices.next(list))
        if (mountedDevices.getParent(list)->mName.isEqual(name))
            return mountedDevices.getParent(list);

    return NULL;
}

FileDevice* FileDeviceMgr::tryOpen(FileHandle* handle, const SafeString& path, FileDevice::FileOpenFlag flag, u32 divSize)
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
    FileDevice* device = findDeviceFromPath(arg.name, &pathNoDrive);

    if (device == NULL)
        return NULL;

    FileDevice::LoadArg arg2(arg);
    arg2.name = pathNoDrive.cstr();

    u8* data = device->tryLoad(arg2);

    arg.fileSize = arg2.fileSize;
    arg.allocSize = arg2.allocSize;
    arg.allocated = arg2.allocated;

    return data;
}

void
FileDeviceMgr::stateChangeCallback_(
    FSClient* client, FSVolumeState state, void* context
)
{
    FSGetLastError(client);
}

}
