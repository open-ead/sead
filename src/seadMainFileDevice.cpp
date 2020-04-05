#include <sead.h>

namespace sead {

MainFileDevice::MainFileDevice(Heap* heap)
    : FileDevice("main")
    , mFileDevice(NULL)
{
#ifdef cafe
    mFileDevice = new(heap, 4) CafeContentFileDevice();
#else
    #error "Unknown platform"
#endif // cafe
}

MainFileDevice::~MainFileDevice()
{
    if (mFileDevice == NULL)
        return;

    delete mFileDevice;
    mFileDevice = NULL;
}

void
MainFileDevice::traceFilePath(
    const SafeString& path
) const
{
    mFileDevice->doTracePath_(path);
}

void
MainFileDevice::traceDirectoryPath(
    const SafeString& path
) const
{
    mFileDevice->doTracePath_(path);
}

void
MainFileDevice::resolveFilePath(
    BufferedSafeString* out, const SafeString& path
) const
{
    mFileDevice->doResolvePath_(out, path);
}

void
MainFileDevice::resolveDirectoryPath(
    BufferedSafeString* out, const SafeString& path
) const
{
    mFileDevice->doResolvePath_(out, path);
}

bool
MainFileDevice::isMatchDevice_(
    const HandleBase* handle
) const
{
    return mFileDevice->isMatchDevice_(handle);
}

bool MainFileDevice::doIsAvailable_() const
{
    return mFileDevice->isAvailable();
}

FileDevice* MainFileDevice::doOpen_(
    FileHandle* handle, const SafeString& path,
    FileDevice::FileOpenFlag flag
)
{
    return mFileDevice->tryOpen(handle, path, flag, handle->mDivSize);
}

bool MainFileDevice::doClose_(
    FileHandle* handle
)
{
    return mFileDevice->tryClose(handle);
}

bool MainFileDevice::doRead_(
    u32* bytesRead, FileHandle* handle,
    u8* outBuffer, u32 bytesToRead
)
{
    return mFileDevice->tryRead(bytesRead, handle, outBuffer, bytesToRead);
}

bool MainFileDevice::doWrite_(
    u32* bytesWritten, FileHandle* handle,
    const u8* inBuffer, u32 bytesToWrite
)
{
    return mFileDevice->tryWrite(bytesWritten, handle, inBuffer, bytesToWrite);
}

bool MainFileDevice::doSeek_(
    FileHandle* handle, s32 offset,
    FileDevice::SeekOrigin origin
)
{
    return mFileDevice->trySeek(handle, offset, origin);
}

bool MainFileDevice::doGetCurrentSeekPos_(
    u32* seekPos, FileHandle* handle
)
{
    return mFileDevice->tryGetCurrentSeekPos(seekPos, handle);
}

bool MainFileDevice::doGetFileSize_(
    u32* fileSize, const SafeString& path
)
{
    return mFileDevice->tryGetFileSize(fileSize, path);
}

bool MainFileDevice::doGetFileSize_(
    u32* fileSize, FileHandle* handle
)
{
    return mFileDevice->tryGetFileSize(fileSize, handle);
}

bool MainFileDevice::doIsExistFile_(
    bool* exists, const SafeString& path
)
{
    return mFileDevice->tryIsExistFile(exists, path);
}

bool MainFileDevice::doIsExistDirectory_(
    bool* exists, const SafeString& path
)
{
    return mFileDevice->tryIsExistDirectory(exists, path);
}

FileDevice* MainFileDevice::doOpenDirectory_(
    DirectoryHandle* handle, const SafeString& path
)
{
    return mFileDevice->tryOpenDirectory(handle, path);
}

bool MainFileDevice::doCloseDirectory_(
    DirectoryHandle* handle
)
{
    return mFileDevice->tryCloseDirectory(handle);
}

bool MainFileDevice::doReadDirectory_(
    u32* entriesRead, DirectoryHandle* handle,
    DirectoryEntry* entries, u32 entriesToRead
)
{
    return mFileDevice->tryReadDirectory(entriesRead, handle, entries, entriesToRead);
}

bool MainFileDevice::doMakeDirectory_(
    const SafeString& path, u32 _
)
{
    return mFileDevice->tryMakeDirectory(path, _);
}

s32 MainFileDevice::doGetLastRawError_() const
{
    return mFileDevice->getLastRawError();
}

} // namespace sead
