#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <sead/seadDisposer.h>
#include <sead/seadResource.h>

namespace sead {

class ResourceMgr
{
public:
    ResourceMgr();

    static ResourceMgr* sInstance;

    IDisposer staticDisposer;
    u32 _10[3];
    u32 _1C[3];
    u32 _28[3];
    DirectResourceFactory<DirectResource>* factory;
};

} // namespace sead

#endif // SEAD_RESOURCEMGR_H_
