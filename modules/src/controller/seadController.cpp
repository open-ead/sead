#include "controller/seadController.h"

#include "controller/seadControllerAddon.h"
#include "controller/seadControllerDefine.h"
#include "controller/seadControllerMgr.h"
#include "controller/seadControllerWrapperBase.h"

namespace sead
{
Controller::Controller(ControllerMgr* mgr)
    : ControllerBase(cPadIdx_Max, cPadIdx_LeftStickUp, cPadIdx_RightStickUp, cPadIdx_Touch),
      mId(ControllerDefine::cController_Null), mMgr(mgr)
{
    mAddons.initOffset(offsetof(ControllerAddon, mListNode));
    mWrappers.initOffset(offsetof(ControllerWrapperBase, mListNode));
}

Controller::~Controller() = default;

void Controller::calc()
{
    u32 prev_hold = getHoldMask();
    bool prev_pointer_on = isPointerOn();

    calcImpl_();

    updateDerivativeParams_(prev_hold, prev_pointer_on);

    bool is_idle = true;

    for (auto it = mAddons.begin(); it != mAddons.end(); ++it)
    {
        if (it->calc())
            is_idle = false;
    }

    if (is_idle && isIdle_())
        mIdleFrame++;
    else
        mIdleFrame = 0;

    for (auto it = mWrappers.begin(); it != mWrappers.end(); ++it)
        it->calc(prev_hold, prev_pointer_on);
}

bool Controller::isConnected() const
{
    return true;
}

ControllerAddon* Controller::getAddon(ControllerDefine::AddonId id) const
{
    return getAddonByOrder(id, 0);
}
ControllerAddon* Controller::getAddonByOrder(ControllerDefine::AddonId id, int index) const
{
    for (auto& addon : mAddons)
    {
        if (addon.mId == id)
        {
            if (index == 0)
                return &addon;
            index--;
        }
    }

    return nullptr;
}

bool Controller::isIdle_()
{
    return isIdleBase_();
}

void Controller::setIdle_()
{
    setIdleBase_();

    for (auto it = mWrappers.begin(); it != mWrappers.end(); ++it)
        it->setIdle();
}

}  // namespace sead
