#include <sead.h>

namespace sead {

u32 FileHandle::read(u8* outBuffer, u32 bytesToRead)
{
    if (device == NULL)
        return 0;

    u32 bytesRead = 0;
    device->tryRead(&bytesRead, this, outBuffer, bytesToRead);
    return bytesRead;
}

FileDevice::~FileDevice()
{
    if (FileDeviceMgr::sInstance != NULL)
        FileDeviceMgr::sInstance->unmount(this);
}

FileDevice*
FileDevice::tryOpen(
    FileHandle* handle, const SafeString& path,
    FileOpenFlag flag, u32 divSize
)
{
    if (this->_4C == 0)
        return NULL;

    if (handle == NULL)
        return NULL;

    setFileHandleDivSize_(handle, divSize);
    FileDevice* device = doOpen_(handle, path, flag);
    setHandleBaseFileDevice_(handle, device);
    if (device != NULL)
        setHandleBaseOriginalFileDevice_(handle, this);

    return device;
}

bool
FileDevice::tryRead(
    u32* bytesRead, FileHandle* handle,
    u8* outBuffer, u32 bytesToRead
)
{
    if (this->_4C == 0)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    if (outBuffer == NULL)
        return false;

    if (handle->divSize == 0)
        return doRead_(bytesRead, handle, outBuffer, bytesToRead);

    u32 totalReadSize = 0;

    do
    {
        u32 readSize = 0;
        u32 size = (static_cast<s32>(bytesToRead) < handle->divSize) ? bytesToRead : handle->divSize;

        if (!doRead_(&readSize, handle, outBuffer, size))
        {
            if (bytesRead != NULL)
                *bytesRead = totalReadSize;

            return false;
        }

        totalReadSize += readSize;
        if (readSize < size)
            break;

        outBuffer += readSize;
        bytesToRead -= size;
    }
    while(bytesToRead != 0);

    if (bytesRead != NULL)
        *bytesRead = totalReadSize;

    return true;
}

u8* FileDevice::tryLoad(LoadArg& arg)
{
    if (this->_4C == 0)
        return NULL;

    return doLoad_(arg);
}

bool
FileDevice::tryClose(FileHandle* handle)
{
    if (this->_4C == 0)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    bool closed = doClose_(handle);
    if (closed)
    {
        setHandleBaseFileDevice_(handle, NULL);
        setHandleBaseOriginalFileDevice_(handle, NULL);
    }

    return closed;
}

void
FileDevice::setFileHandleDivSize_(
    FileHandle* handle, u32 divSize
) const
{
    handle->divSize = divSize;
}

void
FileDevice::setHandleBaseFileDevice_(
    HandleBase* handle, FileDevice* device
) const
{
    handle->device = device;
}

void
FileDevice::setHandleBaseOriginalFileDevice_(
    HandleBase* handle, FileDevice* device
) const
{
    handle->originalDevice = device;
}

} // namespace sead
