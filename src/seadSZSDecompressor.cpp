#include <sead.h>

namespace sead {

SZSDecompressor::SZSDecompressor(u32 workSize, u8* workBuffer)
    : Decompressor("szs")
{
    if (workBuffer == nullptr)
    {
        mWorkBuffer = nullptr;
        mWorkSize = workSize + FileDevice::cBufferMinAlignment - 1 & -FileDevice::cBufferMinAlignment; // workSize + 0x3F & 0xFFFFFFC0 // align(workSize, FileDevice::cBufferMinAlignment)
    }

    else
    {
        mWorkBuffer = workBuffer;
        mWorkSize = workSize;
    }
}

u8*
SZSDecompressor::tryDecompFromDevice(
    ResourceMgr::LoadArg& loadArg, Resource& resource,
    u32* outSize, u32* outAllocSize, bool* success
)
{
    Heap* heap = loadArg.resourceLoadHeap;
    if (heap == nullptr)
        heap = HeapMgr::sInstancePtr->getCurrentHeap();

    FileHandle handle();
    FileDevice* device;

    if (loadArg.device == nullptr)
        device = FileDeviceMgr::sInstance->tryOpen(handle, loadArg.name, 0, loadArg.divSize);
    else
        device = loadArg.device->tryOpen(handle, loadArg.name, 0, loadArg.divSize);

    if (device == nullptr)
        return;

    u8* src = mWorkBuffer;
    if (src == nullptr)
    {
        src = new[](mWorkSize, heap, -FileDevice::cBufferMinAlignment); // src = new[](mWorkSize, heap, -0x40)
        if (src == nullptr)
            return nullptr;
    }

    u32 bytesRead = handle.tryRead(src, mWorkSize);
    if (bytesRead < 0x10)
        goto return_deleteSrc;

    u32 decompSize = getDecompSize(src);
    u32 decompAlignment = getDecompAlignment(src);

    u32 allocSize = loadArg.bufferSize;
    u8* dst = loadArg.buffer;

    if (allocSize < decompSize && allocSize != 0)
        decompSize = allocSize;

    bool decompressed = false;
    allocSize = decompSize + 0x1F & -0x20; // align(decompSize, 0x20)

    if (dst == nullptr)
    {
        // TODO: allocate dst
        goto return_deleteSrc;
    }

    s32 error;

    if (mWorkSize <= bytesRead)
    {
        DecompContext context(dst);
        context.dstSize = decompSize;

        do
        {
            error = streamDecomp(context, src, bytesRead);
            if (error < 0)
                goto return_deleteDst;
        }
        while (error > 0 && (bytesRead = handle.tryRead(src, mWorkSize), bytesRead != 0));

return_success:
        if (mWorkBuffer == nullptr)
            delete[] src;

        if (pOutSize != nullptr)
            *pOutSize = decompSize;

        if (pOutAllocSize != nullptr)
            *pOutAllocSize = allocSize;

        if (pSuccess != nullptr)
            *pSuccess = decompressed;

        return dst;
    }

    error = decomp(dst, allocSize, src, mWorkSize);
    if (error > -1)
        goto return_success;

return_deleteDst:
    if (decompressed)
        delete[] dst;

return_deleteSrc:
    if (mWorkBuffer == nullptr)
        delete[] src;

    return nullptr;
}

} // namespace sead
