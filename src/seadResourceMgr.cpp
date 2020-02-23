#include <sead.h>

namespace sead {

ResourceMgr* ResourceMgr::sInstance = NULL;
ResourceMgr::SingletonDisposer_* ResourceMgr::SingletonDisposer_::sStaticDisposer = NULL;

ResourceMgr::~ResourceMgr()
{
    if (factory == NULL)
        return;

    delete factory;
    factory = NULL;
}

SEAD_CREATE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)
SEAD_DELETE_SINGLETON_INSTANCE(ResourceMgr, ResourceMgr::sInstance)

void ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    if (factory->_C == 0)
        return;

    factory->_C = 0;
    factory->root.erase_();
    _10[2] -= 1;
}

void ResourceMgr::unregisterDecompressor(Decompressor* decompressor)
{
    if (decompressor->_C == 0)
        return;

    decompressor->_C = 0;
    decompressor->root.erase_();
    _28[2] -= 1;
}

} // namespace sead
