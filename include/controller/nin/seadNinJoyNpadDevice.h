#pragma once

#include <nn/hid.h>

#include "container/seadSafeArray.h"
#include "controller/seadControlDevice.h"
#include "controller/seadController.h"
#include "heap/seadHeap.h"
#include "thread/seadCriticalSection.h"
#include "thread/seadThread.h"

namespace sead
{
class NinJoyNpadDevice : public ControlDevice
{
    SEAD_RTTI_OVERRIDE(NinJoyNpadDevice, ControlDevice);

public:
    class VibrationThread : public Thread
    {
    public:
        VibrationThread(Heap* heap);
        ~VibrationThread() override;

        void calc_(s64) override;
        void requestVibration(const nn::hid::VibrationDeviceHandle& handle,
                              const nn::hid::VibrationValue& value);

    private:
        u8 _fc[0x158];
        CriticalSection mCS;
    };

    static_assert(sizeof(VibrationThread) == 0x298);

    struct NpadState
    {
        nn::hid::NpadBaseState mState;
        u8 _28[0x258];
        s32 mSixAxisDeviceNum;
        s32 mVibrationDeviceNum;
        SafeArray<nn::hid::SixAxisSensorHandle, 2> mSixAxisSensorHandles;
        SafeArray<nn::hid::SixAxisSensorState, 2> mSixAxisSensorStates;
        u8 _288[0xb40];
        SafeArray<nn::hid::VibrationDeviceHandle, 2> mVibrationDeviceHandles;
    };

    static_assert(sizeof(NpadState) == 0xe98);

    NinJoyNpadDevice(ControllerMgr* mgr, Heap* heap);
    ~NinJoyNpadDevice() override;

    void calc() override;
    void setNpadIdUpdateNum(u32);
    void setSupportedNpadStyleSet(nn::hid::NpadStyleSet);
    void setNpadJoyHoldType(nn::hid::NpadJoyHoldType);
    nn::hid::NpadJoyAssignmentMode getNpadJoyAssignment(s32);
    void setNpadJoyAssignmentModeSingle(s32);
    void setNpadJoyAssignmentModeSingle(s32, nn::hid::NpadJoyDeviceType);
    void setNpadJoyAssignmentModeDual(s32);
    void mergeSingleJoyAsDualJoy(s32, s32);
    void swapNpadAssignment(s32, s32);
    void disconnectNpad(s32);
    void sendVibrationValue(s32, s32, const nn::hid::VibrationValue&);

    const NpadState& getNpadState(s32 idx) { return mNpadStates[idx]; }

private:
    u32 mNpadIdUpdateNum;
    nn::hid::NpadJoyHoldType mNpadJoyHoldType;
    SafeArray<nn::hid::NpadStyleTag, 9> mNpadStyleTags;
    SafeArray<NpadState, 9> mNpadStates;
    VibrationThread mVibrationThread;
};

}  // namespace sead
