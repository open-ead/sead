#ifndef SEAD_SZS_DECOMPRESSOR_H_
#define SEAD_SZS_DECOMPRESSOR_H_

#include <sead/seadResource.h>
#include <sead/seadResourceMgr.h>
#include <sead/seadDecompressor.h>

namespace sead {

class SZSDecompressor : public Decompressor
{
public:
    SZSDecompressor(u32 workSize, u8* workBuffer);
    virtual ~SZSDecompressor
    {
        if (ResourceMgr::sInstance != nullptr)
            ResourceMgr::sInstance->unregisterDecompressor(this);
    }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource, u32* outSize, u32* outAllocSize, bool* success);

    u32 mWorkSize;
    u8* mWorkBuffer;
}

} // namespace sead

#endif // SEAD_SZS_DECOMPRESSOR_H_
