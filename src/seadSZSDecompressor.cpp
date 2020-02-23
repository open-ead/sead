#include <sead.h>

namespace {

#ifdef cafe
__attribute__((aligned(0x20))) s32 decodeSZSCafeAsm_(void* dst, const void* src)
{
    asm ("lwz r5, 0x4(r4)\n");
    asm ("li r11, 0x20\n");
    asm ("li r6, 0\n");
    asm ("mr r0, r5\n");
    asm ("addi r4, r4, 0xf\n");
    asm ("subi r3, r3, 1\n");
    asm ("cmpwi r5, 0x132\n");
    asm ("ble _final_decloop0\n");
    asm ("subi r5, r5, 0x132\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");
    asm ("nop\n");

    asm ("_decloop0: rlwinm. r6, r6, 0x1f, 1, 0x1f\n");
    asm ("bne _decloop1\n");
    asm ("lbzu r7, 1(r4)\n");
    asm ("li r6, 0x80\n");

    asm ("_decloop1: and. r8, r6, r7\n");
    asm ("lbzu r8, 1(r4)\n");
    asm ("beq _decloop2\n");
    asm ("andi. r9, r3, 0x1f\n");
    asm ("bne _decloop1x\n");
    asm ("dcbz r11, r3\n");

    asm ("_decloop1x: subic. r5, r5, 1\n");
    asm ("stbu r8, 1(r3)\n");
    asm ("bne _decloop0\n");
    asm ("b _decloop8\n");

    asm ("_decloop2: lbzu r9, 1(r4)\n");
    asm ("rlwinm. r10, r8, 0x1c, 4, 0x1f\n");
    asm ("bne _decloop3\n");
    asm ("lbzu r10, 1(r4)\n");
    asm ("addi r10, r10, 0x10");

    asm ("_decloop3: addi r10, r10, 2\n");
    asm ("rlwimi r9, r8, 8, 0x14, 0x17\n");
    asm ("subf r5, r10, r5\n");
    asm ("subf r8, r9, r4\n");
    asm ("mtspr CTR, r10\n");
    asm ("addi r8, r8, 1\n");

    asm ("_decloop4: andi. r9, r3, 0x1f\n");
    asm ("lbz r9, -1(r8)\n");
    asm ("addi r8, r8, 1\n");
    asm ("bne _decloop5\n");
    asm ("dcbz r11, r3\n");

    asm ("_decloop5: stbu r9, 1(r3)\n");
    asm ("bdnz _decloop4\n");
    asm ("cmpwi r5, 0\n");
    asm ("bgt _decloop0\n");

    asm ("_decloop8: addi r5, r5, 0x132\n");
    asm ("cmpwi r5, 0\n");
    asm ("ble _final_decloop8\n");

    asm ("_final_decloop0: rlwinm. r6, r6, 0x1f, 1, 0x1f\n");
    asm ("bne _final_decloop1\n");
    asm ("lbzu r7, 1(r4)\n");
    asm ("li r6, 0x80\n");

    asm ("_final_decloop1: and. r8, r6, r7\n");
    asm ("lbzu r8, 1(r4)\n");
    asm ("beq _final_decloop2\n");
    asm ("subic. r5, r5, 1\n");
    asm ("stbu r8, 1(r3)\n");
    asm ("bne _final_decloop0\n");
    asm ("b _final_decloop8\n");

    asm ("_final_decloop2: lbzu r9, 1(r4)\n");
    asm ("rlwinm. r10, r8, 0x1c, 4, 0x1f\n");
    asm ("bne _final_decloop3\n");
    asm ("lbzu r10, 1(r4)\n");
    asm ("addi r10, r10, 0x10\n");

    asm ("_final_decloop3: addi r10, r10, 2\n");
    asm ("rlwimi r9, r8, 8, 0x14, 0x17\n");
    asm ("subf. r5, r10, r5\n");
    asm ("blt _final_decloop8\n");
    asm ("subf r8, r9, r3\n");
    asm ("mtspr CTR, r10\n");
    asm ("addi r8, r8, 1\n");

    asm ("_final_decloop4: lbz r9, -1(r8)\n");
    asm ("addi r8, r8, 1\n");
    asm ("stbu r9, 1(r3)\n");
    asm ("bdnz _final_decloop4\n");
    asm ("cmpwi r5, 0\n");
    asm ("bgt _final_decloop0\n");

    s32 register error asm("r3");
    asm ("_final_decloop8: mr %0, r0\n" : "=r"(error));
    asm ("blr");

    return error;
}
#endif // cafe

} // namespace sead::{no-name}

namespace sead {

SZSDecompressor::SZSDecompressor(u32 workSize, u8* workBuffer)
    : Decompressor("szs")
{
    if (workBuffer == NULL)
    {
        mWorkSize = workSize + FileDevice::cBufferMinAlignment - 1 & (u32)-FileDevice::cBufferMinAlignment;
        mWorkBuffer = NULL;
    }

    else
    {
        mWorkSize = workSize;
        mWorkBuffer = workBuffer;
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

s32
SZSDecompressor::decomp(void* dst, u32 dstSize, const void* src, u32 srcSize)
{
    u32 magic = Endian::readU32(src);
    if (magic != 0x59617A30)
        return -1;

    u32 decompSize = getDecompSize(src);
    s32 error = -2;
    if (dstSize >= decompSize)
#ifdef cafe
        error = decodeSZSCafeAsm_(dst, src);
#else
        #error "Unknown platform"
#endif // cafe

    return error;
}

} // namespace sead
