#ifndef SEAD_DECOMPRESSOR_H_
#define SEAD_DECOMPRESSOR_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>
#include <sead/seadResource.h>
#include <sead/seadResourceMgr.h>
#include <sead/seadSafeString.h>

namespace sead {

class Decompressor : public UnkList, public IDisposer
{
public:
    Decompressor(SafeString& name)
        : UnkBase()
        , IDisposer()
        , mName(name)
    {
    }

    virtual ~Decompressor
    {
        if (ResourceMgr::sInstance != nullptr)
            ResourceMgr::sInstance->unregisterDecompressor(this);
    }

    virtual u8* tryDecompFromDevice(ResourceMgr::LoadArg& loadArg, Resource& resource, u32* outSize, u32* outAllocSize, bool* success) = 0;

    FixedSafeString<32> mName;
}

} // namespace sead

#endif // SEAD_DECOMPRESSOR_H_
