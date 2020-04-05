#ifndef SEAD_DECOMPRESSOR_H_
#define SEAD_DECOMPRESSOR_H_

#include <sead/seadDisposer.h>
#include <sead/seadResource.h>
#include <sead/seadResourceMgr.h>
#include <sead/seadSafeString.h>
#include <sead/seadTList.h>

namespace sead {

class Decompressor : public TListNode<Decompressor>, public IDisposer
{
public:
    Decompressor(const SafeString& name)
        : TListNode<Decompressor>()
        , IDisposer()
        , mExt(name)
    {
    }

    virtual ~Decompressor()
    {
        if (ResourceMgr::sInstance != NULL)
            ResourceMgr::sInstance->unregisterDecompressor(this);
    }

    virtual u8* tryDecompFromDevice(const ResourceMgr::LoadArg& loadArg, Resource* resource, u32* outSize, u32* outAllocSize, bool* outAllocated) = 0;

    FixedSafeString<32> mExt;
};

} // namespace sead

#endif // SEAD_DECOMPRESSOR_H_
