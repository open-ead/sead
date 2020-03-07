#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <sead/seadDisposer.h>
#include <sead/seadFileDevice.h>
#include <sead/seadHeap.h>
#include <sead/seadSafeString.h>
#include <sead/seadTList.h>

namespace sead {

class Resource;
class DirectResource;
class ResourceFactory;
class Decompressor;

template <typename T>
class DirectResourceFactory;


class ResourceMgr
{
    SEAD_SINGLETON_DISPOSER(ResourceMgr, sInstance)

public:
    struct CreateArg
    {
        u8* buffer;
        u32 bufferSize;
        u32 allocSize;
        bool isValid;
        u32 _10[3];
        Heap* resourceCreateHeap;
        s32 resourceAlignment;
    };

    struct LoadArg
    {
        SafeString name;
        Heap* resourceCreateHeap;
        Heap* resourceLoadHeap;
        s32 resourceAlignment;
        s32 bufferSizeAlignment;
        u8* buffer;
        u32 bufferSize;
        ResourceFactory* factory;
        FileDevice* device;
        u32 divSize;
    };

public:
    ResourceMgr();
    ~ResourceMgr();

    void registerFactory(ResourceFactory* factory, const SafeString& name);
    void registerDecompressor(Decompressor* decompressor, const SafeString& name);

    void unregisterFactory(ResourceFactory* factory);
    void unregisterDecompressor(Decompressor* decompressor);

    static ResourceMgr* sInstance;

    TList<ResourceFactory> factories;
    TList<Resource> postCreateResources;
    TList<Decompressor> decompressors;
    DirectResourceFactory<DirectResource>* factory;
};

} // namespace sead

#endif // SEAD_RESOURCEMGR_H_
