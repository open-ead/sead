#pragma once

#include <mc/seadCoreInfo.h>
#include <resource/seadDecompressor.h>

namespace sead
{
class ParallelSZSDecompressor : public Decompressor
{
public:
    ParallelSZSDecompressor(u32 workSize, s32 threadPriority, sead::Heap* heap, u8* workBuffer,
                            const CoreIdMask& mask);
    ~ParallelSZSDecompressor() override;

    u8* tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource, u32* outSize,
                            u32* outAllocSize, bool* outAllocated) override;

    void setDivSize(u32);

private:
    void* _78[0x1a8 / 8];
};

static_assert(sizeof(ParallelSZSDecompressor) == 0x220);

}  // namespace sead
