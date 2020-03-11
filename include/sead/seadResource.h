#ifndef SEAD_RESOURCE_H_
#define SEAD_RESOURCE_H_

#include <sead/seadDecompressor.h>
#include <sead/seadDisposer.h>
#include <sead/seadHeap.h>
#include <sead/seadListImpl.h>
#include <sead/seadNew.h>
#include <sead/seadResourceMgr.h>

namespace sead {

class Resource : public UnkList
{
    SEAD_RTTI_BASE(Resource)

public:
    Resource();
    virtual ~Resource();

    virtual void doPostCreate_();
};

class DirectResource : public Resource
{
    SEAD_RTTI_OVERRIDE(DirectResource, Resource)

public:
    class Flags
    {
    public:
        explicit Flags()
            : val(0)
        {
        }

        inline void set()
        {
            val |= 1;
        }

        inline void unset()
        {
            val &= ~1;
        }

        inline bool isSet() const
        {
            return val & 1;
        }

    private:
        u32 val;
    };

public:
    DirectResource();
    virtual ~DirectResource();

    virtual s32 getLoadDataAlignment();
    virtual void doCreate_(u8*, u32, Heap*);

    void create(u8* buffer, u32 bufferSize, u32 allocSize, bool allocated, Heap* heap);

    u8* pData;
    u32 dataSize;
    u32 dataAllocSize;
    Flags flags;
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

    virtual ~ResourceFactory();

    virtual Resource* create(const ResourceMgr::CreateArg& createArg) = 0;
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg) = 0;
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor) = 0;

    FixedSafeString<32> mName;
};

class DirectResourceFactoryBase : public ResourceFactory
{
public:
    DirectResourceFactoryBase()
        : ResourceFactory()
    {
    }

    virtual ~DirectResourceFactoryBase()
    {
    }

    virtual Resource* create(const ResourceMgr::CreateArg& createArg);
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg);
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor);
    virtual DirectResource* newResource_(Heap* heap, s32 alignment) = 0;
};

template <typename T>
class DirectResourceFactory : public DirectResourceFactoryBase
{
public:
    DirectResourceFactory()
        : DirectResourceFactoryBase()
    {
    }

    virtual ~DirectResourceFactory()
    {
    }

    virtual DirectResource* newResource_(Heap* heap, s32 alignment)
    {
        return new(heap, alignment) T;
    }
};

} // namespace sead

#endif // SEAD_RESOURCE_H_
