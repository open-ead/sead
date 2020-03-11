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

        __attribute__((always_inline)) bool doCopy(u32 n)
        {
            if (this->headerDstSize < n)
            {
                if (this->dstSize == 0)
                    return false;

                n = this->headerDstSize & 0xFFFF;
            }

            this->headerDstSize -= n;

            do
            {
                *this->dst = *(this->dst - this->copy_pos);
                this->dst += 1;
            }
            while (--n != 0);

            this->action = 0;
            return true;
        }

        u8* dst;
        s32 headerDstSize;
        s32 dstSize;
        u8 codeMask;
        u8 code;
        u8 b1;
        u32 action;
        u16 copy_pos;
        u8 headerSize;
    };

public:
    SZSDecompressor(u32 workSize, u8* workBuffer);
    virtual ~SZSDecompressor() { }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource, u32* outSize, u32* outAllocSize, bool* outAllocated);

    static u32 getDecompAlignment(const void* src);
    static u32 getDecompSize(const void* src);
    static s32 readHeader_(DecompContext* context, const u8* src, u32 srcSize);
    static s32 streamDecomp(DecompContext* context, const void* src, u32 srcSize);
    static s32 decomp(void* dst, u32 dstSize, const void* src, u32 srcSize);

    u32 mWorkSize;
    u8* mWorkBuffer;
};

} // namespace sead

#endif // SEAD_SZS_DECOMPRESSOR_H_
