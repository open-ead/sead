#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <sead/seadDisposer.h>
#include <sead/seadFileDevice.h>
#include <sead/seadHeap.h>
#include <sead/seadSafeString.h>

namespace sead {

class DirectResource;
class ResourceFactory;
class Decompressor;

template <typename T>
class DirectResourceFactory;


class ResourceMgr
{
    SEAD_SINGLETON_DISPOSER(ResourceMgr, sInstance)

public:
    struct CreateArg;

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

    void unregisterFactory(ResourceFactory* factory);
    void unregisterDecompressor(Decompressor* decompressor);

    static ResourceMgr* sInstance;

    u32 _10[3];
    u32 _1C[3];
    u32 _28[3];
    DirectResourceFactory<DirectResource>* factory;
};

} // namespace sead

#endif // SEAD_RESOURCEMGR_H_
