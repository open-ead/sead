#include <sead.h>
#include <stdlib.h>

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

void
FileDevice::traceFilePath(
    const SafeString& path
) const
{
    doTracePath_(path);
}

void
FileDevice::traceDirectoryPath(
    const SafeString& path
) const
{
    doTracePath_(path);
}

void
FileDevice::resolveFilePath(
    BufferedSafeString* out, const SafeString& path
) const
{
    doResolvePath_(out, path);
}

void
FileDevice::resolveDirectoryPath(
    BufferedSafeString* out, const SafeString& path
) const
{
    doResolvePath_(out, path);
}

bool
FileDevice::isMatchDevice_(
    const HandleBase* handle
) const
{
    return handle->device == this;
}

u8* FileDevice::doLoad_(LoadArg& arg)
{
    if (arg.buffer != NULL && arg.bufferSize == 0)
        return NULL;

    FileHandle handle;
    if(tryOpen(&handle, arg.name, FileDevice::FlagRead, arg.divSize))
    {
        u32 fileSize = 0;
        if (tryGetFileSize(&fileSize, &handle))
        {
            u32 bytesToRead = arg.bufferSize;
            if (bytesToRead == 0)
                bytesToRead = MathCalcCommonS32::roundUpPow2(fileSize, FileDevice::cBufferMinAlignment);

            else if (bytesToRead < fileSize)
                goto return_fail;

            u8* buf = arg.buffer;
            bool allocated = false;

            if (buf == NULL)
            {
                s32 sign = (arg.bufferSizeAlignment < 0) ? -1 : 1;
                s32 alignment = abs(arg.bufferSizeAlignment);
                alignment = sign * ((alignment < 0x40) ? 0x40 : alignment);

                buf = new(arg.heap, alignment) u8[bytesToRead];
                allocated = true;
            }

            u32 bytesRead = 0;
            if (tryRead(&bytesRead, &handle, buf, bytesToRead) && tryClose(&handle))
            {
                arg.fileSize = bytesRead;
                arg.allocSize = bytesToRead;
                arg.allocated = allocated;

                return buf;
            }

            else if (allocated)
                delete[] buf;
        }
    }

return_fail:
    return NULL;
}

void
FileDevice::doTracePath_(
    const SafeString& path
) const
{
    FixedSafeString<512> out;
    doResolvePath_(&out, path);
}

void
FileDevice::doResolvePath_(
    BufferedSafeString* out, const SafeString& path
) const
{
    out->copy(path);
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

bool
FileDevice::tryGetFileSize(
    u32* fileSize, FileHandle* handle
)
{
    if (this->_4C == 0)
        return false;

    if (handle == NULL)
        return false;

    if (fileSize == NULL)
        return false;

    return doGetFileSize_(fileSize, handle);
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
