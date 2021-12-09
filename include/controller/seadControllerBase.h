#pragma once

#include "math/seadBoundBox.h"
#include "math/seadVector.h"
#include "prim/seadRuntimeTypeInfo.h"
#include "prim/seadTypedBitFlag.h"

namespace sead
{
class ControllerBase
{
    SEAD_RTTI_BASE(ControllerBase)
public:
    ControllerBase(int, int, int, int);

    void setRightStickCrossThreshold(float, float);
    void setPointerBound(const BoundBox2<float>&);
    void setPadRepeat(u32, u8, u8);
    void setLeftStickCrossThreshold(float, float);
    // unknown return type
    u32 getPadHoldCount(int);

    int getButtonsTrigger() { return mButtonsTrigger; }
    int getButtonsRelease() { return mButtonsRelease; }
    int getButtonsRepeat() { return mButtonsRepeat; }
    int getButtonsHold() { return mButtonsHold; }
    Vector2<float>* getTouchScreenPos() { return &mTouchScreenPos; }
    Vector2<float>* getLeftJoy() { return &mLeftJoy; }
    Vector2<float>* getRightJoy() { return &mRightJoy; }

protected:
    void updateDerivativeParams_(u32, bool);
    void setPointerWithBound_(bool, bool, const Vector2f& bound);
    void setIdleBase_();
    bool isIdleBase_();
    // unknown return type
    u32 getStickHold_(u32, const Vector2f&, float, float, int);
    // unknown return type
    u32 createStickCrossMask_();

private:
    int mButtonsTrigger;
    int mButtonsRelease;
    unsigned int mButtonsRepeat;
    unsigned int mFlags;
    int _18;
    int _1c;
    BoundBox2<float> mPointerBound;
    int mPadHoldCounts[32];
    char _b0[32];
    char _d0[32];
    float mLeftStickThresholdX;
    float mRightStickThresholdX;
    float mLeftStickThresholdY;
    float mRightStickThresholdY;
    int _100;
    int _104;
    int _108;
    int _10c;
    unsigned int mIdleCounter;
    unsigned int mButtonsHold;
    Vector2<float> mTouchScreenPos;
    Vector2<float> mLeftJoy;
    Vector2<float> mRightJoy;
    Vector2<float> _130;
};

}  // namespace sead
