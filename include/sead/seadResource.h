#ifndef SEAD_RESOURCE_H_
#define SEAD_RESOURCE_H_

#include <sead/seadDecompressor.h>
#include <sead/seadDisposer.h>
#include <sead/seadHeap.h>
#include <sead/seadListImpl.h>

namespace sead {

class Resource : public UnkList
{
    SEAD_RTTI_BASE(Resource)

public:
    Resource();
    virtual ~Resource() { }

    virtual void doPostCreate_();
};

class DirectResource : public Resource
{
    SEAD_RTTI_OVERRIDE(DirectResource, Resource)

public:
    DirectResource();
    virtual ~DirectResource() { }

    virtual s32 getLoadDataAlignment();
    virtual void doCreate_(u8*, u32, Heap*);

    u8* pData;
    u32 dataSize;
    u32 _1C;
    u32 flags[1];
};

class ResourceFactory : public UnkList, public IDisposer
{
public:
    ResourceFactory()
        : UnkList()
        , IDisposer()
        , mName()
    {
    }

    virtual ~ResourceFactory
    {
        if (ResourceMgr::sInstance != nullptr)
            ResourceMgr::sInstance->unregisterFactory(this);
    }

    virtual Resource* create(const ResourceMgr::CreateArg& createArg) = 0;
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg) = 0;
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor) = 0;

    FixedSafeString<32> mName;
};

class DirectResourceFactoryBase : ResourceFactory
{
public:
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg);
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor);
};

template <typename T>
class DirectResourceFactory : public DirectResourceFactoryBase
{
public:
    DirectResourceFactory()
        : ResourceFactory()
    {
    }

    virtual ~DirectResourceFactory { }

    virtual T* newResource_(Heap* heap, s32 alignment)
    {
        return new(sizeof(T), heap, alignment) T;
    }
};

} // namespace sead

#endif // SEAD_RESOURCE_H_
