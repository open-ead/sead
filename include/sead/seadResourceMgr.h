#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <sead/seadDecompressor.h>
#include <sead/seadDisposer.h>
#include <sead/seadFileDevice.h>
#include <sead/seadHeap.h>
#include <sead/seadResource.h>
#include <sead/seadSafeString.h>

namespace sead {

class ResourceMgr
{
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

    void unregisterFactory(ResourceFactory*);
    void unregisterDecompressor(Decompressor*);

    static ResourceMgr* sInstance;

    IDisposer staticDisposer;
    u32 _10[3];
    u32 _1C[3];
    u32 _28[3];
    DirectResourceFactory<DirectResource>* factory;
};

} // namespace sead

#endif // SEAD_RESOURCEMGR_H_
