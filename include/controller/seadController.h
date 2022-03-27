#pragma once

#include "container/seadOffsetList.h"
#include "controller/seadControllerBase.h"
#include "controller/seadControllerDefine.h"

namespace sead
{
class ControllerAddon;
class ControllerMgr;
class ControllerWrapperBase;

class Controller : public ControllerBase
{
    SEAD_RTTI_OVERRIDE(Controller, ControllerBase)
public:
    enum PadIdx
    {
        cPadIdx_A = 0,
        cPadIdx_B = 1,
        cPadIdx_C = 2,
        cPadIdx_X = 3,
        cPadIdx_Y = 4,
        cPadIdx_Z = 5,
        cPadIdx_2 = 6,  // Also Right-Stick Click
        cPadIdx_1 = 7,  // Also Left-Stick Click
        cPadIdx_Home = 8,
        cPadIdx_Minus = 9,
        cPadIdx_Plus = 10,
        cPadIdx_Start = 11,
        cPadIdx_Select = 12,
        cPadIdx_ZL = cPadIdx_C,
        cPadIdx_ZR = cPadIdx_Z,
        cPadIdx_L = 13,
        cPadIdx_R = 14,
        cPadIdx_Touch = 15,
        cPadIdx_Up = 16,
        cPadIdx_Down = 17,
        cPadIdx_Left = 18,
        cPadIdx_Right = 19,
        cPadIdx_LeftStickUp = 20,
        cPadIdx_LeftStickDown = 21,
        cPadIdx_LeftStickLeft = 22,
        cPadIdx_LeftStickRight = 23,
        cPadIdx_RightStickUp = 24,
        cPadIdx_RightStickDown = 25,
        cPadIdx_RightStickLeft = 26,
        cPadIdx_RightStickRight = 27,
        cPadIdx_Max = 28
    };

    explicit Controller(ControllerMgr* mgr);
    virtual ~Controller();

    virtual void calc();
    virtual bool isConnected() const;
    ControllerAddon* getAddonByOrder(ControllerDefine::AddonId id, int index) const;
    ControllerAddon* getAddon(ControllerDefine::AddonId id) const;

    template <typename T>
    T getAddonAs() const;

protected:
    virtual void calcImpl_() = 0;
    virtual bool isIdle_();
    virtual void setIdle_();

private:
    ControllerDefine::ControllerId mId;
    ControllerMgr* mMgr;
    OffsetList<ControllerAddon> mAddons;
    OffsetList<ControllerWrapperBase> mWrappers;

    friend class ControllerWrapperBase;
    friend class ControllerMgr;
};
#ifdef cafe
static_assert(sizeof(Controller) == 0x15C, "sead::Controller size mismatch");
#endif  // cafe

template <typename T>
T Controller::getAddonAs() const
{
    for (auto& addon : mAddons)
    {
        T result = DynamicCast<std::remove_pointer<T>>(addon);
        if (result)
            return result;
    }

    return nullptr;
}

}  // namespace sead
