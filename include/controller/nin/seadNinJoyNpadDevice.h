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
    u8 filler[0x8620];
};

}  // namespace sead
