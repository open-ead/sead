#pragma once

#include "controller/seadControllerAddon.h"
#include "math/seadVector.h"

namespace sead
{
class Controller;

class AccelerometerAddon : public ControllerAddon
{
    SEAD_RTTI_OVERRIDE(AccelerometerAddon, ControllerAddon)

public:
    explicit AccelerometerAddon(Controller* controller)
        : ControllerAddon(controller), mIsEnable(false), mAcceleration(0.0f, 0.0f, 0.0f)
    {
        mId = ControllerDefine::cAddon_Accelerometer;
    }

    ~AccelerometerAddon() override = default;

    bool isEnable() const { return mIsEnable; }
    const Vector3f& getAcceleration() const { return mAcceleration; }

protected:
    bool mIsEnable;
    Vector3f mAcceleration;
};
#ifdef cafe
static_assert(sizeof(AccelerometerAddon) == 0x24, "sead::AccelerometerAddon size mismatch");
#endif  // cafe

}  // namespace sead
