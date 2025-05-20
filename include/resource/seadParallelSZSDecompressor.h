#pragma once

#include <mc/seadCoreInfo.h>
#include <resource/seadDecompressor.h>

namespace sead
{
class ParallelSZSDecompressor : public Decompressor
{
public:
    ParallelSZSDecompressor(u32, s32, sead::Heap*, u8*, const CoreIdMask&);
    ~ParallelSZSDecompressor() override;

    u8* tryDecompFromDevice(const sead::ResourceMgr::LoadArg&, sead::Resource*, u32*, u32*,
                            bool*) override;

    void setDivSize(u32);

private:
    void* size[0x1a8 / 8];
};

static_assert(sizeof(ParallelSZSDecompressor) == 0x220);

}  // namespace sead
