#pragma once

#include "controller/seadControlDevice.h"
#include "controller/seadController.h"
#include "heap/seadHeap.h"

namespace sead
{
// stubbed and only added methods for seadControllerMgr
class NinJoyNpadDevice : public ControlDevice
{
public:
    NinJoyNpadDevice(ControllerMgr* mgr, Heap* heap);

    void calc() override;

private:
    void* filler[4292];
};

}  // namespace sead
