#include "controller/seadControllerMgr.h"
#include "basis/seadNew.h"
#include "controller/nin/seadNinJoyNpadDevice.h"
#include "controller/seadControlDevice.h"
#include "framework/seadTaskID.h"
#include "prim/seadDelegate.h"
#include "thread/seadDelegateThread.h"

#ifdef cafe
#include <controller/cafe/seadCafeDRCControllerCafe.h>
#include <controller/cafe/seadCafeDRCPatternRumbleAddonCafe.h>
#include <controller/cafe/seadCafeRemoteControllerCafe.h>
#include <controller/cafe/seadCafeRemotePatternRumbleAddonCafe.h>
#include <controller/cafe/seadCafeVPadDeviceCafe.h>
#include <controller/cafe/seadCafeWPadDeviceCafe.h>
#endif  // cafe

namespace sead
{
SEAD_TASK_SINGLETON_IMPL(ControllerMgr)

// NON_MATCHING: storing too much 00s into stack (for ConstructArg)
ControllerMgr::ControllerMgr() : CalculateTask(ConstructArg(), "sead::ControllerMgr")
{
    mDevices.initOffset(offsetof(ControlDevice, mListNode));
}

ControllerMgr::ControllerMgr(const TaskConstructArg& arg)
    : CalculateTask(arg, "sead::ControllerMgr")
{
    mDevices.initOffset(offsetof(ControlDevice, mListNode));
}

ControllerMgr::~ControllerMgr() = default;

void ControllerMgr::prepare()
{
    auto* parameter = DynamicCast<Parameter>(mParameter);
    if (parameter)
    {
        initialize(parameter->controllerMax, nullptr);
        if (parameter->proc)
            parameter->proc->invoke(this);
    }
    else
    {
        initializeDefault(nullptr);
    }
}

void ControllerMgr::initialize(s32 controller_max, Heap* heap)
{
    mControllers.allocBuffer(controller_max, heap);
}

void ControllerMgr::finalize()
{
    mControllers.freeBuffer();
}

void ControllerMgr::initializeDefault(Heap* heap)
{
    s32 controller_max;
#ifdef cafe
    controller_max = 6;
#elif defined(NNSDK)
    controller_max = 16;
#else
#error "Unknown Platform"
#endif
    initialize(controller_max, heap);

#ifdef NNSDK
    mDevices.pushBack(new (heap) NinJoyNpadDevice(this, heap));
#endif
}

void ControllerMgr::finalizeDefault()
{
#ifdef cafe
    // TODO: CafeDebugPadDevice, CafeDebugController

    //{
    //    ControlDevice* device = getControlDevice(ControllerDefine::cDevice_CafeDebugPad);
    //    if (device)
    //    {
    //        mDevices.erase(device);
    //        delete device;
    //    }
    //    else
    //    {
    //        //SEAD_ASSERT_MSG(false, "CafeDebugPadDevice is not found.");
    //    }
    //
    //    {
    //        Controller* controller = getControllerByOrder(ControllerDefine::cController_CafeDebug,
    //        0); if (controller)
    //        {
    //            mControllers.erase(mControllers.indexOf(controller));
    //            delete controller;
    //        }
    //        else
    //        {
    //            //SEAD_ASSERT_MSG(false, "CafeDebugController is not found.");
    //        }
    //    }
    //}

    {
        ControlDevice* device = getControlDevice(ControllerDefine::cDevice_CafeWPad);
        if (device)
        {
            mDevices.erase(device);
            delete device;
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "CafeWPadDevice is not found.");
        }

        for (u32 i = 0; i < 4; i++)
        {
            Controller* controller =
                getControllerByOrder(ControllerDefine::cController_CafeRemote, 0);
            if (controller)
            {
                ControllerAddon* addon =
                    controller->getAddon(ControllerDefine::cAddon_PatternRumble);
                if (addon)
                {
                    delete addon;
                }
                else
                {
                    // SEAD_ASSERT_MSG(false, "CafeRemotePatternRumbleAddon[%d] is not found.", i);
                }

                mControllers.erase(mControllers.indexOf(controller));
                delete controller;
            }
            else
            {
                // SEAD_ASSERT_MSG(false, "CafeRemoteController[%d] is not found.", i);
            }
        }
    }

    {
        {
            Controller* controller = getControllerByOrder(ControllerDefine::cController_CafeDRC, 0);
            if (controller)
            {
                // Dear Nintendo,
                // You forgot to delete the CafeDRCPatternRumbleAddon instance

                mControllers.erase(mControllers.indexOf(controller));
                delete controller;
            }
            else
            {
                // SEAD_ASSERT_MSG(false, "CafeDRCController is not found.");
            }
        }

        ControlDevice* device = getControlDevice(ControllerDefine::cDevice_CafeVPad);
        if (device)
        {
            mDevices.erase(device);
            delete device;
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "CafeVPadDevice is not found.");
        }
    }
#elif defined(NNSDK)

    // NON_MATCHING: missing cbz instruction within loop
    for (OffsetList<ControlDevice>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        ControlDevice* device = &(*it);
        if (device->getId() == 13)
        {
            mDevices.erase(device);
            delete device;
            break;
        }
    }
#endif  // cafe

    finalize();
}

// NON_MATCHING: swapped ldr / ldrsw on second loop check
void ControllerMgr::calc()
{
    for (OffsetList<ControlDevice>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
        it->calc();

    for (PtrArray<Controller>::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
        it->calc();
}

Controller* ControllerMgr::getControllerByOrder(ControllerDefine::ControllerId id, s32 index) const
{
    for (PtrArray<Controller>::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
    {
        Controller& controller = *it;
        if (controller.mId == id)
        {
            if (index == 0)
                return &controller;

            index--;
        }
    }

    return nullptr;
}

ControlDevice* ControllerMgr::getControlDevice(ControllerDefine::DeviceId id) const
{
    for (OffsetList<ControlDevice>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        ControlDevice& device = *it;
        if (device.mId == id)
            return &device;
    }

    return nullptr;
}

ControllerAddon* ControllerMgr::getControllerAddon(s32 index, ControllerDefine::AddonId id) const
{
    Controller* controller = mControllers.at(index);
    if (controller)
        return controller->getAddon(id);

    return nullptr;
}

ControllerAddon* ControllerMgr::getControllerAddonByOrder(s32 controller_index,
                                                          ControllerDefine::AddonId id,
                                                          s32 addon_index) const
{
    Controller* controller = mControllers.at(controller_index);
    if (controller)
        return controller->getAddonByOrder(id, addon_index);

    return nullptr;
}

s32 ControllerMgr::findControllerPort(const Controller* controller) const
{
    SEAD_ASSERT(controller);

    s32 i = 0;
    for (PtrArray<Controller>::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
    {
        if (&(*it) == controller)
            return i;
        i++;
    }
    return -1;
}

DelegateThread* ControllerMgr::getFramework() const
{
    if (mTaskMgr)
        return mTaskMgr->mPrepareThread;
    return nullptr;
}

}  // namespace sead
