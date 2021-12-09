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
    ControllerMgr(const TaskConstructArg&);
    ControllerMgr();

    void calc() override;
    void finalize();
    void finalizeDefault();
    int findControllerPort(const Controller*);
    NinJoyNpadDevice* getControlDevice(ControllerDefine::DeviceId);
    void* getControllerAddon(int, ControllerDefine::AddonId);              // unknown return type
    void* getControllerAddonByOrder(int, ControllerDefine::AddonId, int);  // unknown return type
    Controller* getControllerByOrder(ControllerDefine::ControllerId, int);
    void* getFramework();  // unknown return type, probably inherited from TaskBase
    void initialize(int, Heap*);
    void initializeDefault(Heap*);
    void prepare() override;


    Controller* getController(int port) {return mArray[port];}

private:
    OffsetList<NinJoyNpadDevice> mList;
    PtrArray<Controller> mArray;
};

}  // namespace sead