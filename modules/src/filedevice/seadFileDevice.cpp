#include <basis/seadNew.h>
#include <filedevice/seadFileDevice.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <math/seadMathCalcCommon.hpp>
#include <stdlib.h>

namespace sead
{
u32 FileHandle::read(u8* outBuffer, u32 bytesToRead)
{
    if (mDevice == NULL)
        return 0;

    u32 bytesRead = 0;
    mDevice->tryRead(&bytesRead, this, outBuffer, bytesToRead);
    return bytesRead;
}

FileDevice::~FileDevice()
{
    if (FileDeviceMgr::instance() != NULL)
        FileDeviceMgr::instance()->unmount(this);
}

void FileDevice::traceFilePath(const SafeString& path) const
{
    doTracePath_(path);
}

void FileDevice::traceDirectoryPath(const SafeString& path) const
{
    doTracePath_(path);
}

void FileDevice::resolveFilePath(BufferedSafeString* out, const SafeString& path) const
{
    doResolvePath_(out, path);
}

void FileDevice::resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const
{
    doResolvePath_(out, path);
}

bool FileDevice::isMatchDevice_(const HandleBase* handle) const
{
    return handle->mDevice == this;
}

u8* FileDevice::doLoad_(LoadArg& arg)
{
    if (arg.buffer != NULL && arg.buffer_size == 0)
        return NULL;

    FileHandle handle;
    if (tryOpen(&handle, arg.path, FileDevice::cFileOpenFlag_ReadOnly, arg.div_size))
    {
        u32 fileSize = 0;
        if (tryGetFileSize(&fileSize, &handle))
        {
            u32 bytesToRead = arg.buffer_size;
            if (bytesToRead == 0)
                bytesToRead =
                    MathCalcCommonS32::roundUpPow2(fileSize, FileDevice::cBufferMinAlignment);

            else if (bytesToRead < fileSize)
                goto return_fail;

            u8* buf = arg.buffer;
            bool allocated = false;

            if (buf == NULL)
            {
                s32 sign = (arg.alignment < 0) ? -1 : 1;
                s32 alignment = abs(arg.alignment);
                alignment = sign * ((alignment < 0x40) ? 0x40 : alignment);

                buf = new (arg.heap, alignment) u8[bytesToRead];
                allocated = true;
            }

            u32 bytesRead = 0;
            if (tryRead(&bytesRead, &handle, buf, bytesToRead) && tryClose(&handle))
            {
                arg.read_size = bytesRead;
                arg.roundup_size = bytesToRead;
                arg.need_unload = allocated;

                return buf;
            }

            else if (allocated)
                delete[] buf;
        }
    }

return_fail:
    return NULL;
}

void FileDevice::doTracePath_(const SafeString& path) const
{
    FixedSafeString<512> out;
    doResolvePath_(&out, path);
}

void FileDevice::doResolvePath_(BufferedSafeString* out, const SafeString& path) const
{
    out->copy(path);
}

bool FileDevice::isAvailable() const
{
    if (!mPermission)
        return false;

    return doIsAvailable_();
}

u8* FileDevice::tryLoad(LoadArg& arg)
{
    if (!mPermission)
        return NULL;

    return doLoad_(arg);
}

FileDevice* FileDevice::tryOpen(FileHandle* handle, const SafeString& path, FileOpenFlag flag,
                                u32 divSize)
{
    if (!mPermission)
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

bool FileDevice::tryClose(FileHandle* handle)
{
    if (!mPermission)
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

bool FileDevice::tryRead(u32* bytesRead, FileHandle* handle, u8* outBuffer, u32 bytesToRead)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    if (outBuffer == NULL)
        return false;

    if (handle->mDivSize == 0)
        return doRead_(bytesRead, handle, outBuffer, bytesToRead);

    u32 totalReadSize = 0;

    do
    {
        u32 readSize = 0;
        u32 size =
            (static_cast<s32>(bytesToRead) < handle->mDivSize) ? bytesToRead : handle->mDivSize;

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
    } while (bytesToRead != 0);

    if (bytesRead != NULL)
        *bytesRead = totalReadSize;

    return true;
}

bool FileDevice::tryWrite(u32* bytesWritten, FileHandle* handle, const u8* inBuffer,
                          u32 bytesToWrite)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (inBuffer == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    return doWrite_(bytesWritten, handle, inBuffer, bytesToWrite);
}

bool FileDevice::trySeek(FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    return doSeek_(handle, offset, origin);
}

bool FileDevice::tryGetCurrentSeekPos(u32* seekPos, FileHandle* handle)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    if (seekPos == NULL)
        return false;

    return doGetCurrentSeekPos_(seekPos, handle);
}

bool FileDevice::tryGetFileSize(u32* fileSize, const SafeString& path)
{
    if (!mPermission)
        return false;

    if (fileSize == NULL)
        return false;

    return doGetFileSize_(fileSize, path);
}

bool FileDevice::tryGetFileSize(u32* fileSize, FileHandle* handle)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (fileSize == NULL)
        return false;

    return doGetFileSize_(fileSize, handle);
}

bool FileDevice::tryIsExistFile(bool* exists, const SafeString& path)
{
    if (!mPermission)
        return false;

    if (exists == NULL)
        return false;

    return doIsExistFile_(exists, path);
}

bool FileDevice::tryIsExistDirectory(bool* exists, const SafeString& path)
{
    if (!mPermission)
        return false;

    if (exists == NULL)
        return false;

    return doIsExistDirectory_(exists, path);
}

FileDevice* FileDevice::tryOpenDirectory(DirectoryHandle* handle, const SafeString& path)
{
    if (!mPermission)
        return NULL;

    if (handle == NULL)
        return NULL;

    FileDevice* device = doOpenDirectory_(handle, path);
    setHandleBaseFileDevice_(handle, device);
    if (device != NULL)
        setHandleBaseOriginalFileDevice_(handle, this);

    return device;
}

bool FileDevice::tryCloseDirectory(DirectoryHandle* handle)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    bool closed = doCloseDirectory_(handle);
    if (closed)
    {
        setHandleBaseFileDevice_(handle, NULL);
        setHandleBaseOriginalFileDevice_(handle, NULL);
    }

    return closed;
}

bool FileDevice::tryReadDirectory(u32* entriesRead, DirectoryHandle* handle,
                                  DirectoryEntry* entries, u32 entriesToRead)
{
    if (!mPermission)
        return false;

    if (handle == NULL)
        return false;

    if (!isMatchDevice_(handle))
        return false;

    u32 readCount = 0;
    bool success = doReadDirectory_(&readCount, handle, entries, entriesToRead);

    if (entriesRead != NULL)
        *entriesRead = readCount;

    if (readCount > entriesToRead)
        return false;

    return success;
}

bool FileDevice::tryMakeDirectory(const SafeString& path, u32 _)
{
    if (!mPermission)
        return false;

    return doMakeDirectory_(path, _);
}

s32 FileDevice::getLastRawError() const
{
    return doGetLastRawError_();
}

void FileDevice::setFileHandleDivSize_(FileHandle* handle, u32 divSize) const
{
    handle->mDivSize = divSize;
}

void FileDevice::setHandleBaseFileDevice_(HandleBase* handle, FileDevice* device) const
{
    handle->mDevice = device;
}

void FileDevice::setHandleBaseOriginalFileDevice_(HandleBase* handle, FileDevice* device) const
{
    handle->mOriginalDevice = device;
}

}  // namespace sead
