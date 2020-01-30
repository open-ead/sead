#ifndef SEAD_RESOURCE_H_
#define SEAD_RESOURCE_H_

#include <sead/seadHeap.h>
#include <sead/seadListImpl.h>

namespace sead {

class Resource : public UnkList
{
    SEAD_RTTI_BASE(Resource)

public:
    Resource();
    virtual ~Resource() { }

    virtual void doPostCreate_();
};

class DirectResource : public Resource
{
    SEAD_RTTI_OVERRIDE(DirectResource, Resource)

public:
    DirectResource();
    virtual ~DirectResource() { }

    virtual s32 getLoadDataAlignment();
    virtual void doCreate_(u8*, u32, Heap*);

    u8* pData;
    u32 dataSize;
    u32 _1C;
    u32 flags[1];
};

} // namespace sead

#endif // SEAD_RESOURCE_H_
