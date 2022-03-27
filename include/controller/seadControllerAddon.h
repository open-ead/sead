#pragma once

#include "container/seadListImpl.h"
#include "controller/seadControllerDefine.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class Controller;

class ControllerAddon
{
    SEAD_RTTI_BASE(ControllerAddon)

public:
    explicit ControllerAddon(Controller* controller)
        : mId(ControllerDefine::cAddon_Null), mController(controller)
    {
    }

    virtual ~ControllerAddon() = default;

    virtual bool calc() = 0;

protected:
    ListNode mListNode;
    ControllerDefine::AddonId mId;
    Controller* mController;

    friend class Controller;
};
#ifdef cafe
static_assert(sizeof(ControllerAddon) == 0x14, "sead::ControllerAddon size mismatch");
#endif  // cafe

}  // namespace sead
