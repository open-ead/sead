#ifndef SEAD_RESOURCE_H_
#define SEAD_RESOURCE_H_

#include <sead/seadBitFlag.h>
#include <sead/seadDecompressor.h>
#include <sead/seadDisposer.h>
#include <sead/seadHeap.h>
#include <sead/seadNew.h>
#include <sead/seadResourceMgr.h>
#include <sead/seadTList.h>
#include <sead/types.h>

namespace sead
{
class Resource
{
public:
    SEAD_RTTI_BASE(Resource)

    Resource();
    virtual ~Resource();
};

class DirectResource : public Resource
{
    SEAD_RTTI_OVERRIDE(DirectResource, Resource)

public:
    DirectResource();
    virtual ~DirectResource();

    virtual s32 getLoadDataAlignment() const;
    virtual void doCreate_(u8* buffer, u32 bufferSize, Heap* heap);

    void create(u8* buffer, u32 bufferSize, u32 allocSize, bool allocated, Heap* heap);

    u8* mRawData = 0;
    u32 mRawSize = 0;
    u32 mBufferSize = 0;
    BitFlag32 mSettingFlag;
};

class ResourceFactory : public TListNode<ResourceFactory>, public IDisposer
{
public:
    ResourceFactory() : TListNode<ResourceFactory>(), IDisposer(), mExt() {}

    virtual ~ResourceFactory();

    virtual Resource* create(const ResourceMgr::CreateArg& createArg) = 0;
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg) = 0;
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg,
                                          Decompressor* decompressor) = 0;

    FixedSafeString<32> mExt;
};

class DirectResourceFactoryBase : public ResourceFactory
{
public:
    DirectResourceFactoryBase() : ResourceFactory() {}

    virtual ~DirectResourceFactoryBase() {}

    virtual Resource* create(const ResourceMgr::CreateArg& createArg);
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg);
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg,
                                          Decompressor* decompressor);
    virtual DirectResource* newResource_(Heap* heap, s32 alignment) = 0;
};

template <typename T>
class DirectResourceFactory : public DirectResourceFactoryBase
{
public:
    DirectResourceFactory() : DirectResourceFactoryBase() {}

    virtual ~DirectResourceFactory() {}

    virtual DirectResource* newResource_(Heap* heap, s32 alignment)
    {
        return new (heap, alignment) T;
    }
};

}  // namespace sead

#endif  // SEAD_RESOURCE_H_
