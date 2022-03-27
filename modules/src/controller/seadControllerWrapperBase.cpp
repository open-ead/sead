#include "controller/seadControllerWrapperBase.h"

#include "prim/seadMemUtil.h"

namespace sead
{
ControllerWrapperBase::ControllerWrapperBase()
    : ControllerBase(Controller::cPadIdx_Max, -1, -1, Controller::cPadIdx_Touch),
      mController(nullptr), mIsEnable(true)
{
}

ControllerWrapperBase::~ControllerWrapperBase()
{
    unregister();
}

void ControllerWrapperBase::registerWith(Controller* controller, bool copy_repeat_setting)
{
    SEAD_ASSERT(controller);

    unregister();

    controller->mWrappers.pushBack(this);
    mController = controller;

    if (copy_repeat_setting)
        copyRepeatSetting(controller);
}

void ControllerWrapperBase::unregister()
{
    if (mController)
    {
        mController->mWrappers.erase(this);
        mController = nullptr;
    }
}

void ControllerWrapperBase::copyRepeatSetting(const Controller* controller)
{
    MemUtil::copy(mPadRepeatDelays, controller->mPadRepeatDelays, 32);
    MemUtil::copy(mPadRepeatPulses, controller->mPadRepeatPulses, 32);
}

void ControllerWrapperBase::setEnable(bool enable)
{
    mIsEnable = enable;
}

void ControllerWrapperBase::setEnableOtherWrappers(bool enable) const
{
    if (!mController)
        return;

    OffsetList<ControllerWrapperBase>& wrappers = mController->mWrappers;
    for (auto it = wrappers.begin(); it != wrappers.end(); ++it)
    {
        ControllerWrapperBase& wrapper = *it;
        if (&wrapper != this)
            wrapper.setEnable(enable);
    }
}

void ControllerWrapperBase::setIdle()
{
    setIdleBase_();
}

bool ControllerWrapperBase::isIdle_()
{
    return isIdleBase_();
}

}  // namespace sead
