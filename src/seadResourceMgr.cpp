#include <sead.h>

namespace sead {

ResourceMgr* ResourceMgr::sInstance = NULL;
ResourceMgr::SingletonDisposer_* ResourceMgr::SingletonDisposer_::sStaticDisposer = NULL;

ResourceMgr::ResourceMgr()
    : factories()
    , _1C()
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

    parent = &factories;
    factory->parent = parent;
    parent->mStartEnd.insertFront_(&factory->root);
    factories.mCount += 1;
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(sead::SafeString::cEmptyString))
        decompressor->mName.copy(name);

    ListImpl* parent = decompressor->parent;
    if (parent != NULL)
    {
        decompressor->parent = NULL;
        decompressor->root.erase_();
        parent->mCount -= 1;
    }

    parent = &decompressors;
    decompressor->parent = parent;
    parent->mStartEnd.insertFront_(&decompressor->root);
    decompressors.mCount += 1;
}

void ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    if (factory->parent == NULL)
        return;

    factory->parent = NULL;
    factory->root.erase_();
    factories.mCount -= 1;
}

void ResourceMgr::unregisterDecompressor(Decompressor* decompressor)
{
    if (decompressor->parent == NULL)
        return;

    decompressor->parent = NULL;
    decompressor->root.erase_();
    decompressors.mCount -= 1;
}

} // namespace sead
