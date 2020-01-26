#ifndef SEAD_DECOMPRESSOR_H_
#define SEAD_DECOMPRESSOR_H_

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
