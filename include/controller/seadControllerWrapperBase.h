#pragma once

#include "controller/seadController.h"
#include "heap/seadDisposer.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class Controller;

class ControllerWrapperBase : public ControllerBase, public IDisposer
{
    SEAD_RTTI_OVERRIDE(ControllerWrapperBase, ControllerBase)

public:
    ControllerWrapperBase();
    virtual ~ControllerWrapperBase();

    virtual void calc(u32 prev_hold, bool prev_pointer_on) = 0;
    virtual void setIdle();

protected:
    virtual bool isIdle_();

public:
    void registerWith(Controller* controller, bool copy_repeat_setting);
    void unregister();
    void copyRepeatSetting(const Controller* controller);
    void setEnable(bool enable);
    void setEnableOtherWrappers(bool enable) const;

protected:
    Controller* mController;
    bool mIsEnable;
    ListNode mListNode;
    u8 mPadConfig[cPadIdx_MaxBase];

    friend class Controller;
};
#ifdef cafe
static_assert(sizeof(ControllerWrapperBase) == 0x174, "sead::ControllerWrapperBase size mismatch");
#endif  // cafe

}  // namespace sead
