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
    u8* src;

    if (loadArg.device != NULL)
        device = loadArg.device->tryOpen(&handle, loadArg.name, (FileDevice::FileOpenFlag)0, loadArg.divSize);
    else
        device = FileDeviceMgr::sInstance->tryOpen(&handle, loadArg.name, (FileDevice::FileOpenFlag)0, loadArg.divSize);

    if (device != NULL &&
       ((src = mWorkBuffer, src != NULL) ||
        (src = new(heap, -FileDevice::cBufferMinAlignment) u8[mWorkSize], src != NULL)))
    {
        u32 bytesRead = handle.tryRead(src, mWorkSize);
        if (bytesRead >= 0x10)
        {
            u32 decompSize = getDecompSize(src);
            s32 decompAlignment = getDecompAlignment(src);

            u32 allocSize = loadArg.bufferSize;
            u8* dst = loadArg.buffer;

            if (decompSize > allocSize && allocSize != 0)
                decompSize = allocSize;

            bool decompressed = false;
            allocSize = decompSize + 0x1F & (u32)-0x20;

            s32 alignSign;
            s32 alignment;

            if (dst == NULL)
            {
                if (!IsDerivedFrom<DirectResource, Resource>(resource))
                {
                    alignSign = 1;
                    if (loadArg.resourceAlignment < 0)
                        alignSign = -1;

                    dst = new(heap, -(alignSign << 5)) u8[allocSize];
                }

                else
                {
                    if (resource != NULL)
                    {
                        alignment = loadArg.bufferSizeAlignment;
                        if (alignment != 0)
                        {
                            if (alignment < 0x20)
                                alignment = 0x20;

                            dst = new(heap, alignment) u8[allocSize];
                        }

                        else
                        {
                            if (decompAlignment == 0)
                            {
                                decompAlignment = static_cast<DirectResource*>(resource)->getLoadDataAlignment();
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
                    }
                }

                if (dst != NULL)
                    decompressed = true;
            }


            if (dst != NULL)
            {
                s32 error;
                if (bytesRead < mWorkSize)
                    error = decomp(dst, allocSize, src, mWorkSize);

                else
                {
                    DecompContext context(dst);
                    context.dstSize = decompSize;

                    do
                    {
                        error = streamDecomp(&context, src, bytesRead);
                        if (error <= 0)
                            break;
                    }
                    while ((bytesRead = handle.tryRead(src, mWorkSize), bytesRead != 0));
                }

                if (!(error < 0))
                {
                    if (mWorkBuffer == NULL)
                        delete[] src;

                    if (outSize != NULL)
                        *outSize = decompSize;

                    if (outAllocSize != NULL)
                        *outAllocSize = allocSize;

                    if (success != NULL)
                        *success = decompressed;

                    return dst;
                }

                if (decompressed)
                    delete[] dst;
            }
        }

        if (mWorkBuffer == NULL)
            delete[] src;
    }

    return NULL;
}

u32
SZSDecompressor::getDecompAlignment(const void* src)
{
    return Endian::readU32(src + 8);
}

u32
SZSDecompressor::getDecompSize(const void* src)
{
    return Endian::readU32(src + 4);
}

} // namespace sead
