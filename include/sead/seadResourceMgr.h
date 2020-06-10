#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <sead/seadDisposer.h>
#include <sead/seadFileDevice.h>
#include <sead/seadHeap.h>
#include <sead/seadSafeString.hpp>
#include <sead/seadTList.h>
#include <sead/types.h>

namespace sead
{
class Resource;
class DirectResource;
class ResourceFactory;
class Decompressor;

template <typename T>
class DirectResourceFactory;

class ResourceMgr
{
    SEAD_SINGLETON_DISPOSER(ResourceMgr)

public:
    struct CreateArg
    {
        u8* buffer = nullptr;
        u32 file_size = 0;
        u32 buffer_size = 0;
        bool need_unload = false;
        ResourceFactory* factory = nullptr;
        SafeString ext;
        Heap* heap = nullptr;
        s32 alignment = 0x20;
    };
#ifdef SWITCH
    static_assert(sizeof(CreateArg) == 0x40);
#endif

    struct LoadArg
    {
        SafeString path;
        Heap* instance_heap = nullptr;
        Heap* load_data_heap = nullptr;
        s32 instance_alignment = 0x20;
        s32 load_data_alignment = 0;
        u8* load_data_buffer = nullptr;
        u32 load_data_buffer_size = 0;
        s32 load_data_buffer_alignment = 0;
        ResourceFactory* factory = nullptr;
        FileDevice* device = nullptr;
        // Read chunk size.
        u32 div_size = 0;
        bool _4C = true;
        bool* has_tried_create_with_decomp = nullptr;
    };
#ifdef SWITCH
    static_assert(sizeof(LoadArg) == 0x58);
#endif

public:
    ResourceMgr();
    ~ResourceMgr();

    void registerFactory(ResourceFactory* factory, const SafeString& name);
    void unregisterFactory(ResourceFactory* factory);
    void setDefaultFactory(ResourceFactory* factory);
    ResourceFactory* findFactory(const SafeString& name);

    void registerDecompressor(Decompressor* decompressor, const SafeString& name);
    void unregisterDecompressor(Decompressor* decompressor);
    Decompressor* findDecompressor(const SafeString& name);

    Resource* tryLoad(const LoadArg& arg, const SafeString&, Decompressor* decompressor);
    Resource* tryLoadWithoutDecomp(const LoadArg& arg);
    void unload(Resource* res);

    typedef TList<ResourceFactory*> FactoryList;
    typedef TList<Decompressor*> DecompressorList;

    FactoryList mFactoryList;
    DecompressorList mDecompList;
    ResourceFactory* mNullResourceFactory = nullptr;
    ResourceFactory* mDefaultResourceFactory = nullptr;
};
#ifdef SWITCH
static_assert(sizeof(ResourceMgr) == 0x60);
#endif

}  // namespace sead

#endif  // SEAD_RESOURCEMGR_H_
