#include <basis/seadRawPrint.h>
#include <heap/seadHeapMgr.h>
#include <resource/seadResource.h>
#include <resource/seadResourceMgr.h>

namespace sead
{
SEAD_SINGLETON_DISPOSER_IMPL(ResourceMgr)

ResourceMgr::ResourceMgr()
{
    if (HeapMgr::sInstancePtr == NULL)
    {
        SEAD_ASSERT_MSG(false, "ResourceMgr need HeapMgr");
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

void ResourceMgr::registerFactory(ResourceFactory* factory, const SafeString& name)
{
    factory->mExt.copy(name);

    mFactoryList.pushBack(factory);
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        decompressor->mExt.copy(name);

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
