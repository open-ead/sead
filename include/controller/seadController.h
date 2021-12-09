#pragma once

#include "container/seadOffsetList.h"
#include "controller/seadControllerBase.h"

namespace sead
{
class ControllerMgr;
class ControllerAddon;

namespace ControllerDefine
{
enum AddonId : int {};
enum ControllerId : int {_15 = 15, _16 = 16};
enum DeviceId : int {};

}  // namespace ControllerDefine

class Controller : public ControllerBase
{
    SEAD_RTTI_BASE(Controller)
public:
    Controller(ControllerMgr*);
    virtual ~Controller();
    virtual void calc();
    virtual bool isConnected();
    virtual void calcImpl_() = 0;
    virtual bool isIdle_();
    virtual void setIdle_();
    ControllerAddon* getAddonByOrder(ControllerDefine::AddonId, int);
    ControllerAddon* getAddon(ControllerDefine::AddonId);

private:
    int mControllerId;
    ControllerMgr* mMgr;
    OffsetList<ControllerAddon> mAddonList;
    OffsetList<void*> _160; //unknown type
};

}  // namespace sead
