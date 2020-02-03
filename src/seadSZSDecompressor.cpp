#include <sead.h>

namespace sead {

SZSDecompressor::SZSDecompressor(u32 workSize, u8* workBuffer)
    : Decompressor("szs")
{
    if (workBuffer == NULL)
    {
        mWorkBuffer = NULL;
        mWorkSize = workSize + FileDevice::cBufferMinAlignment - 1 & (u32)-FileDevice::cBufferMinAlignment;
    }

    else
    {
        mWorkBuffer = workBuffer;
        mWorkSize = workSize;
    }
}

u8*
SZSDecompressor::tryDecompFromDevice(
    const ResourceMgr::LoadArg& loadArg, Resource* resource,
    u32* outSize, u32* outAllocSize, bool* success
)
{
    Heap* heap = loadArg.resourceLoadHeap;
    if (heap == NULL)
        heap = HeapMgr::sInstancePtr->getCurrentHeap();

    FileHandle handle;
    FileDevice* device;

    if (loadArg.device == NULL)
        device = FileDeviceMgr::sInstance->tryOpen(&handle, loadArg.name, (FileDevice::FileOpenFlag)0, loadArg.divSize);
    else
        device = loadArg.device->tryOpen(&handle, loadArg.name, (FileDevice::FileOpenFlag)0, loadArg.divSize);

    if (device == NULL)
        return NULL;

    u8* src = mWorkBuffer;
    if (src == NULL)
    {
        src = new(heap, -FileDevice::cBufferMinAlignment) u8[mWorkSize];
        if (src == NULL)
            return NULL;
    }

    u32 bytesRead = handle.tryRead(src, mWorkSize);
    if (bytesRead < 0x10)
        goto return_deleteSrc;

    u32 decompSize = getDecompSize(src);
    s32 decompAlignment = getDecompAlignment(src);

    u32 allocSize = loadArg.bufferSize;
    u8* dst = loadArg.buffer;

    if (allocSize < decompSize && allocSize != 0)
        decompSize = allocSize;

    bool decompressed = false;
    allocSize = decompSize + 0x1F & (u32)-0x20;

    s32 alignSign;
    s32 alignment;

    if (dst == NULL)
    {
        if (!(IsDerivedFrom<DirectResource, Resource>(resource) && resource != NULL))
        {
            alignSign = 1;
            if (loadArg.resourceAlignment < 0)
                alignSign = -1;

            dst = new(heap, alignSign * -0x20) u8[allocSize];
        }

        else
        {
            DirectResource* directResource = reinterpret_cast<DirectResource*>(resource);
            alignment = loadArg.bufferSizeAlignment;
            if (alignment == 0)
            {
                if (decompAlignment == 0)
                {
                    decompAlignment = directResource->getLoadDataAlignment();
                    alignment = loadArg.resourceAlignment;
                }

                else
                    alignment = loadArg.resourceAlignment;

                alignSign = 1;
                if (alignment < 0)
                    alignSign = -1;

                if (decompAlignment < 0x20)
                    decompAlignment = 0x20;

                dst = new(heap, alignSign * decompAlignment) u8[allocSize];
            }

            else
            {
                if (alignment < 0x20)
                    alignment = 0x20;

                dst = new(heap, alignment) u8[allocSize];
            }
        }

        if (dst == NULL)
            goto return_deleteSrc;

        decompressed = true;
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
        if (mWorkBuffer == NULL)
            delete[] src;

        if (pOutSize != NULL)
            *pOutSize = decompSize;

        if (pOutAllocSize != NULL)
            *pOutAllocSize = allocSize;

        if (pSuccess != NULL)
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
    if (mWorkBuffer == NULL)
        delete[] src;

    return NULL;
}

} // namespace sead
