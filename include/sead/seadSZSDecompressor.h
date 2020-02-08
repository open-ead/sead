#ifndef SEAD_SZS_DECOMPRESSOR_H_
#define SEAD_SZS_DECOMPRESSOR_H_

#include <sead/seadResource.h>
#include <sead/seadResourceMgr.h>
#include <sead/seadDecompressor.h>

namespace sead {

class SZSDecompressor : public Decompressor
{
public:
    class DecompContext
    {
    public:
        DecompContext();
        DecompContext(void* dst);

        void initialize(void* dst);

        void* dst;
        s32 headerDstSize;
        s32 dstSize;
        u8 _C;
        u8 _D;
        u8 _E;
        u32 _10;
        u16 _14;
        u8 headerSize;
    };

public:
    SZSDecompressor(u32 workSize, u8* workBuffer);
    virtual ~SZSDecompressor() { }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource, u32* outSize, u32* outAllocSize, bool* success);

    static u32 getDecompAlignment(const void*);
    static u32 getDecompSize(const void*);
    static s32 streamDecomp(DecompContext* context, const void* src, u32 srcSize);
    static s32 decomp(void* dst, u32 dstSize, const void* src, u32 srcSize);

    u32 mWorkSize;
    u8* mWorkBuffer;
};

} // namespace sead

#endif // SEAD_SZS_DECOMPRESSOR_H_
