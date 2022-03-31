#pragma once

#include "container/seadListImpl.h"
#include "controller/seadControllerDefine.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class ControllerMgr;

class ControlDevice
{
    SEAD_RTTI_BASE(ControlDevice)

public:
    explicit ControlDevice(ControllerMgr* mgr) : mId(ControllerDefine::cDevice_Null), mMgr(mgr) {}

    virtual ~ControlDevice() = default;

    virtual void calc() = 0;

    ControllerDefine::DeviceId getId() const { return mId; }

protected:
    ListNode mListNode;
    ControllerDefine::DeviceId mId;
    ControllerMgr* mMgr;

    friend class ControllerMgr;
};
#ifdef cafe
static_assert(sizeof(ControlDevice) == 0x14, "sead::ControlDevice size mismatch");
#endif  // cafe

}  // namespace sead
