#pragma once

#include "container/seadOffsetList.h"
#include "container/seadPtrArray.h"
#include "controller/seadController.h"
#include "framework/seadCalculateTask.h"
#include "framework/seadTaskMgr.h"
#include "heap/seadDisposer.h"

namespace sead
{
class Controller;
class NinJoyNpadDevice;

class ControllerMgr : public CalculateTask
{
    SEAD_TASK_SINGLETON(ControllerMgr)
public:
    explicit ControllerMgr(const TaskConstructArg& arg);
    ControllerMgr();

    void calc() override;
    void finalize();
    void finalizeDefault();
    int findControllerPort(const Controller* controller);
    NinJoyNpadDevice* getControlDevice(ControllerDefine::DeviceId deviceId);
    // unknown return type
    void* getControllerAddon(int, ControllerDefine::AddonId addonId);
    // unknown return type
    void* getControllerAddonByOrder(int, ControllerDefine::AddonId addonId, int);
    Controller* getControllerByOrder(ControllerDefine::ControllerId controllerId, int);
    // unknown return type, probably inherited from TaskBase
    void* getFramework();
    void initialize(int, Heap*);
    void initializeDefault(Heap*);
    void prepare() override;

    Controller* getController(int port) { return mArray[port]; }

private:
    OffsetList<NinJoyNpadDevice> mList;
    PtrArray<Controller> mArray;
};

}  // namespace sead
