#pragma once

#include "math/seadBoundBox.h"
#include "math/seadVector.h"
#include "prim/seadTypedBitFlag.h"
#include "prim/seadRuntimeTypeInfo.h"

namespace sead
{
class ControllerBase
{
    SEAD_RTTI_BASE(ControllerBase)
public:
    ControllerBase(int, int, int, int);

    void updateDerivativeParams_(u32, bool);
    void setRightStickCrossThreshold(float, float);
    void setPointerWithBound_(bool, bool, const sead::Vector2f&);
    void setPointerBound(const sead::BoundBox2<float>&);
    void setPadRepeat(u32, u8, u8);
    void setLeftStickCrossThreshold(float, float);
    void setIdleBase_();
    bool isIdleBase_();
    u32 getStickHold_(u32, const sead::Vector2f&, float, float, int); //unknown return type
    u32 getPadHoldCount(int); //unknown return type
    u32 createStickCrossMask_(); //unknown return type

private:
    int mButtonsTrigger;
    int mButtonsRelease;
    unsigned int mButtonsRepeat;
    unsigned int mFlags;
    int field_18;
    int field_1C;
    sead::BoundBox2<float> mPointerBound;
    int mPadHoldCounts[32];
    char field_B0[32];
    unsigned char field_D0[32];
    float mLeftStickThresholdX;
    float mRightStickThresholdX;
    float mLeftStickThresholdY;
    float mRightStickThresholdY;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    unsigned int mIdleCounter;
    unsigned int mButtonsHold;
    sead::Vector2<float> mTouchScreenPos;
    sead::Vector2<float> mLeftJoy;
    sead::Vector2<float> mRightJoy;
    sead::Vector2<float> field_130;
};

}  // namespace sead
