#include <basis/seadRawPrint.h>
#include <heap/seadHeapMgr.h>
#include <resource/seadResource.h>
#include <resource/seadResourceMgr.h>

namespace sead
{
ResourceMgr* ResourceMgr::sInstance = NULL;
ResourceMgr::SingletonDisposer_* ResourceMgr::SingletonDisposer_::sStaticDisposer = NULL;

ResourceMgr::ResourceMgr()
{
    if (HeapMgr::sInstancePtr == NULL)
    {
        SEAD_ASSERT(false, "ResourceMgr need HeapMgr");
        return;
    }

    mNullResourceFactory =
        new (HeapMgr::sInstancePtr->findContainHeap(this)) DirectResourceFactory<DirectResource>();
    mDefaultResourceFactory = mNullResourceFactory;
    registerFactory(mNullResourceFactory, "");
}

ResourceMgr::~ResourceMgr()
{
    if (mNullResourceFactory == NULL)
        return;

    delete mNullResourceFactory;
    mNullResourceFactory = NULL;
}

SEAD_CREATE_SINGLETON_INSTANCE(ResourceMgr)
SEAD_DELETE_SINGLETON_INSTANCE(ResourceMgr)

void ResourceMgr::registerFactory(ResourceFactory* factory, const SafeString& name)
{
    factory->mExt.copy(name);

    FactoryList* list = factory->mList;
    if (list != NULL)
        list->erase(factory);
    mFactoryList.pushBack(factory);
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        decompressor->mExt.copy(name);

    DecompressorList* list = decompressor->mList;
    if (list != NULL)
        list->erase(decompressor);
    mDecompList.pushBack(decompressor);
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
