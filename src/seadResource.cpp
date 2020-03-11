#include <sead.h>

namespace sead {

Resource::Resource()
    : UnkList()
{
}

Resource::~Resource()
{
}

void Resource::doPostCreate_()
{
}

DirectResource::DirectResource()
    : Resource()
    , pData(NULL)
    , dataSize(0)
    , dataAllocSize(0)
    , flags()
{
}

DirectResource::~DirectResource()
{
    if (flags.isSet())
        delete[] pData;
}

s32 DirectResource::getLoadDataAlignment()
{
    return 4;
}

void DirectResource::doCreate_(u8*, u32, Heap*)
{
}

void DirectResource::create(u8* buffer, u32 bufferSize, u32 allocSize, bool allocated, Heap* heap)
{
    if (pData != NULL)
        return;

    dataSize = bufferSize;
    dataAllocSize = allocSize;
    pData = buffer;

    if (allocated)
        flags.set();

    else
        flags.unset();

    return doCreate_(buffer, bufferSize, heap);
}

ResourceFactory::~ResourceFactory()
{
    if (ResourceMgr::sInstance != NULL)
        ResourceMgr::sInstance->unregisterFactory(this);
}

Resource* DirectResourceFactoryBase::create(const ResourceMgr::CreateArg& createArg)
{
    DirectResource* resource = newResource_(createArg.resourceCreateHeap, createArg.resourceAlignment);
    if (resource == NULL)
        return NULL;

    size_t bufferPtr = reinterpret_cast<size_t>(createArg.buffer);
    s32 alignment = resource->getLoadDataAlignment();
    if (bufferPtr % alignment != 0)
    {
        delete resource;
        return NULL;
    }

    resource->create(createArg.buffer, createArg.bufferSize, createArg.allocSize, createArg.allocated, createArg.resourceCreateHeap);
    return resource;
}

Resource* DirectResourceFactoryBase::tryCreate(const ResourceMgr::LoadArg& loadArg)
{
    DirectResource* resource = newResource_(loadArg.resourceCreateHeap, loadArg.resourceAlignment);
    if (resource == NULL)
        return NULL;

    FileDevice::LoadArg fileLoadArg;
    u8* data;

    fileLoadArg.name = loadArg.name;
    fileLoadArg.buffer = loadArg.buffer;
    fileLoadArg.bufferSize = loadArg.bufferSize;
    fileLoadArg.heap = loadArg.resourceLoadHeap;
    fileLoadArg.divSize = loadArg.divSize;

    if (loadArg.bufferSizeAlignment != 0)
        fileLoadArg.bufferSizeAlignment = loadArg.bufferSizeAlignment;

    else
        fileLoadArg.bufferSizeAlignment = ((loadArg.resourceAlignment < 0)? -1: 1) * resource->getLoadDataAlignment();

    if (loadArg.device != NULL)
        data = loadArg.device->tryLoad(fileLoadArg);

    else
        data = FileDeviceMgr::sInstance->tryLoad(fileLoadArg);

    if (data == NULL)
    {
        delete resource;
        return NULL;
    }

    resource->create(data, fileLoadArg.fileSize, fileLoadArg.allocSize, fileLoadArg.allocated, loadArg.resourceCreateHeap);
    return resource;
}

Resource*
DirectResourceFactoryBase::tryCreateWithDecomp(
    const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor
)
{
    DirectResource* resource = newResource_(loadArg.resourceCreateHeap, loadArg.resourceAlignment);
    if (resource == NULL)
        return NULL;

    u32 outSize = 0;
    u32 outAllocSize = 0;
    bool outAllocated = false;

    u8* data = decompressor->tryDecompFromDevice(loadArg, resource, &outSize, &outAllocSize, &outAllocated);

    resource->create(data, outSize, outAllocSize, outAllocated, loadArg.resourceCreateHeap);
    return resource;
}

}
