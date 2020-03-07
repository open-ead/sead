#include <sead.h>

namespace sead {

ResourceMgr* ResourceMgr::sInstance = NULL;
ResourceMgr::SingletonDisposer_* ResourceMgr::SingletonDisposer_::sStaticDisposer = NULL;

ResourceMgr::ResourceMgr()
    : factories()
    , postCreateResources()
    , decompressors()
    , factory(NULL)
{
    if (HeapMgr::sInstancePtr == NULL)
        return;

    factory = new(HeapMgr::sInstancePtr->findContainHeap(this), 4) DirectResourceFactory<DirectResource>();
    registerFactory(factory, "");
}

ResourceMgr::~ResourceMgr()
{
    if (factory == NULL)
        return;

    delete factory;
    factory = NULL;
}

SEAD_CREATE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)
SEAD_DELETE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)

void ResourceMgr::registerFactory(ResourceFactory* factory, const SafeString& name)
{
    factory->mName.copy(name);

    ListImpl* parent = factory->parent;
    if (parent != NULL)
    {
        factory->parent = NULL;
        factory->root.erase_();
        parent->mCount -= 1;
    }

    factories.setParentFor(factory);
    factories.insertFront(factory);
    factories.getCountRef() += 1;
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        decompressor->mName.copy(name);

    ListImpl* parent = decompressor->parent;
    if (parent != NULL)
    {
        decompressor->parent = NULL;
        decompressor->root.erase_();
        parent->mCount -= 1;
    }

    decompressors.setParentFor(decompressor);
    decompressors.insertFront(decompressor);
    decompressors.getCountRef() += 1;
}

void ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    if (factory->parent == NULL)
        return;

    factory->parent = NULL;
    factory->root.erase_();
    factories.getCountRef() -= 1;
}

void ResourceMgr::unregisterDecompressor(Decompressor* decompressor)
{
    if (decompressor->parent == NULL)
        return;

    decompressor->parent = NULL;
    decompressor->root.erase_();
    decompressors.getCountRef() -= 1;
}

} // namespace sead
