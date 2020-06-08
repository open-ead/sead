#include <sead/seadHeapMgr.h>
#include <sead/seadResource.h>
#include <sead/seadResourceMgr.h>

namespace sead
{
ResourceMgr* ResourceMgr::sInstance = NULL;
ResourceMgr::SingletonDisposer_* ResourceMgr::SingletonDisposer_::sStaticDisposer = NULL;

ResourceMgr::ResourceMgr()
    : mFactoryList(), mPostCreateResourceList(), mDecompList(), mNullResourceFactory(NULL)
{
    if (HeapMgr::sInstancePtr == NULL)
        return;

    mNullResourceFactory = new (HeapMgr::sInstancePtr->findContainHeap(this), 4)
        DirectResourceFactory<DirectResource>();
    registerFactory(mNullResourceFactory, "");
}

ResourceMgr::~ResourceMgr()
{
    if (mNullResourceFactory == NULL)
        return;

    delete mNullResourceFactory;
    mNullResourceFactory = NULL;
}

SEAD_CREATE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)
SEAD_DELETE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)

void ResourceMgr::registerFactory(ResourceFactory* factory, const SafeString& name)
{
    factory->mExt.copy(name);

    FactoryList* list = factory->mList;
    if (list != NULL)
        list->erase(factory);
    mFactoryList.insertFront(factory);
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        decompressor->mExt.copy(name);

    DecompressorList* list = decompressor->mList;
    if (list != NULL)
        list->erase(decompressor);
    mDecompList.insertFront(decompressor);
}

void ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    if (factory->mList == NULL)
        return;
    mFactoryList.erase(factory);
}

void ResourceMgr::unregisterDecompressor(Decompressor* decompressor)
{
    if (decompressor->mList == NULL)
        return;
    mDecompList.erase(decompressor);
}

}  // namespace sead
