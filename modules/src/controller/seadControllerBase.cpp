#include "controller/seadControllerBase.h"

namespace sead
{
const f32 ControllerBase::cStickHoldThresholdDefault = 0.5f;
const f32 ControllerBase::cStickReleaseThresholdDefault = 0.25f;

const Vector2f ControllerBase::cInvalidPointer(Mathf::minNumber(), Mathf::minNumber());
const Vector2i ControllerBase::cInvalidPointerS32(Mathi::minNumber(), Mathi::minNumber());

ControllerBase::ControllerBase(s32 padBitMax, s32 leftStickCrossStartBit,
                               s32 rightStickCrossStartBit, s32 touchKeyBit)
    : mPadTrig(), mPadRelease(), mPadRepeat(), mPointerFlag(), mPointerS32(cInvalidPointerS32),
      mPointerBound(), mLeftStickHoldThreshold(0.5f), mRightStickHoldThreshold(0.5f),
      mLeftStickReleaseThreshold(0.25f), mRightStickReleaseThreshold(0.25f), mPadBitMax(padBitMax),
      mLeftStickCrossStartBit(leftStickCrossStartBit),
      mRightStickCrossStartBit(rightStickCrossStartBit), mTouchKeyBit(touchKeyBit), mIdleFrame(0),
      mPadHold(), mPointer(cInvalidPointer), mLeftStick(0.0f, 0.0f), mRightStick(0.0f, 0.0f),
      mLeftAnalogTrigger(0.0f), mRightAnalogTrigger(0.0f)
{
    if (cPadIdx_MaxBase < padBitMax)
    {
        SEAD_ASSERT_MSG(false, "illegal padBitMax[%d]", padBitMax);
        mPadBitMax = cPadIdx_MaxBase;
    }

    for (u32 i = 0; i < cPadIdx_MaxBase; i++)
    {
        mPadRepeatDelays[i] = 30;
        mPadRepeatPulses[i] = 1;
        mPadHoldCounts[i] = 0;
    }
}

void ControllerBase::setPointerWithBound_(bool is_on, bool touchkey_hold, const Vector2f& pos)
{
    if (is_on)
    {
        if (!mPointerBound.isUndef())
        {
            if (mPointerBound.isInside(pos))
            {
                mPointer.x = pos.x - mPointerBound.getMin().x;
                mPointer.y = pos.y - mPointerBound.getMin().y;
            }
            else
            {
                is_on = false;
            }
        }
        else
        {
            mPointer.x = pos.x;
            mPointer.y = pos.y;
        }
    }

    mPointerFlag.change(cPointerOn, is_on);

    if (mTouchKeyBit >= 0)
    {
        mPadHold.changeBit(mTouchKeyBit, is_on && touchkey_hold);
    }

    if (mPointerFlag.isOn(cPointerUnkFlag3))
    {
        if (mPointerFlag.isOff(cPointerOn))
        {
            mPointer.x = cInvalidPointer.x;
            mPointer.y = cInvalidPointer.y;
        }

        mPointerFlag.reset(cPointerUnkFlag3);
    }
}

void ControllerBase::updateDerivativeParams_(u32 prev_hold, bool prev_pointer_on)
{
    u32 stick_hold = 0;

    if (mLeftStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mLeftStick, mLeftStickHoldThreshold,
                                    mLeftStickReleaseThreshold, mLeftStickCrossStartBit);
    }

    if (mRightStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mRightStick, mRightStickHoldThreshold,
                                    mRightStickReleaseThreshold, mRightStickCrossStartBit);
    }

    mPadHold.setDirect((mPadHold.getDirect() & ~createStickCrossMask_()) | stick_hold);

    mPadTrig.setDirect(~prev_hold & mPadHold.getDirect());
    mPadRelease.setDirect(prev_hold & ~mPadHold.getDirect());
    mPadRepeat.setDirect(0);

    for (s32 i = 0; i < mPadBitMax; i++)
    {
        if (mPadHold.isOnBit(i))
        {
            if (mPadRepeatPulses[i])
            {
                if (mPadHoldCounts[i] == mPadRepeatDelays[i])
                    mPadRepeat.setBit(i);

                else if (mPadRepeatDelays[i] < mPadHoldCounts[i] &&
                         (mPadHoldCounts[i] - mPadRepeatDelays[i]) % mPadRepeatPulses[i] == 0)
                {
                    mPadRepeat.setBit(i);
                }
            }

            mPadHoldCounts[i]++;
        }
        else
        {
            mPadHoldCounts[i] = 0;
        }
    }

    mPointerFlag.change(cPointerOnNow, !prev_pointer_on && mPointerFlag.isOn(cPointerOn));
    mPointerFlag.change(cPointerOffNow, prev_pointer_on && mPointerFlag.isOff(cPointerOn));

    mPointerS32.x = (s32)mPointer.x;
    mPointerS32.y = (s32)mPointer.y;
}

u32 ControllerBase::getPadHoldCount(s32 bit) const
{
    SEAD_ASSERT(bit < mPadBitMax);
    return mPadHoldCounts[bit];
}

void ControllerBase::setPadRepeat(u32 mask, u8 delay_frame, u8 pulse_frame)
{
    BitFlag32 pad_to_set(mask);

    for (s32 i = 0; i < mPadBitMax; i++)
    {
        if (pad_to_set.isOnBit(i))
        {
            mPadRepeatDelays[i] = delay_frame;
            mPadRepeatPulses[i] = pulse_frame;
        }
    }
}

void ControllerBase::setLeftStickCrossThreshold(f32 hold, f32 release)
{
    if (hold >= release)
    {
        mLeftStickHoldThreshold = hold;
        mLeftStickReleaseThreshold = release;
    }
    else
    {
        SEAD_ASSERT_MSG(false, "hold[%f] must be larger than or equal to release[%f].", hold,
                        release);
    }
}

void ControllerBase::setRightStickCrossThreshold(f32 hold, f32 release)
{
    if (hold >= release)
    {
        mRightStickHoldThreshold = hold;
        mRightStickReleaseThreshold = release;
    }
    else
    {
        SEAD_ASSERT_MSG(false, "hold[%f] must be larger than or equal to release[%f].", hold,
                        release);
    }
}

void ControllerBase::setPointerBound(const BoundBox2f& bound)
{
    mPointerBound.set(bound.getMin(), bound.getMax());
    mPointerFlag.set(cPointerUnkFlag3);
}

u32 ControllerBase::getStickHold_(u32 prev_hold, const Vector2f& stick, f32 hold_threshold,
                                  f32 release_threshold, s32 start_bit)
{
    f32 length = stick.length();

    if (length < release_threshold ||
        (length < hold_threshold &&
         (prev_hold & (1 << (start_bit + cCrossUp) | 1 << (start_bit + cCrossDown) |
                       1 << (start_bit + cCrossLeft) | 1 << (start_bit + cCrossRight))) == 0))
    {
        return 0;
    }
    else
    {
        u32 angle = Mathf::atan2Idx(stick.y, stick.x);

        if (angle < 0x10000000)
        {
            return 1 << (start_bit + cCrossRight);
        }
        else if (angle < 0x30000000)
        {
            return 1 << (start_bit + cCrossRight) | 1 << (start_bit + cCrossUp);
        }
        else if (angle < 0x50000000)
        {
            return 1 << (start_bit + cCrossUp);
        }
        else if (angle < 0x70000000)
        {
            return 1 << (start_bit + cCrossLeft) | 1 << (start_bit + cCrossUp);
        }
        else if (angle < 0x90000000)
        {
            return 1 << (start_bit + cCrossLeft);
        }
        else if (angle < 0xb0000000)
        {
            return 1 << (start_bit + cCrossLeft) | 1 << (start_bit + cCrossDown);
        }
        else if (angle < 0xd0000000)
        {
            return 1 << (start_bit + cCrossDown);
        }
        else if (angle < 0xf0000000)
        {
            return 1 << (start_bit + cCrossRight) | 1 << (start_bit + cCrossDown);
        }
        else
        {
            return 1 << (start_bit + cCrossRight);
        }
    }
}

bool ControllerBase::isIdleBase_()
{
    return getHoldMask() == 0 && mPointerFlag.isOff(1) && mLeftStick.isZero() &&
           mRightStick.isZero() && mLeftAnalogTrigger == 0.0f && mRightAnalogTrigger == 0.0f;
}

void ControllerBase::setIdleBase_()
{
    mPadHold.makeAllZero();
    mPadTrig.makeAllZero();
    mPadRelease.makeAllZero();
    mPadRepeat.makeAllZero();
    mPointerFlag.makeAllZero();

    for (s32 i = 0; i < mPadBitMax; i++)
        mPadHoldCounts[i] = 0;

    mPointer.set(cInvalidPointer);
    mPointerS32.set(cInvalidPointerS32);
    mLeftStick.set(0.0f, 0.0f);
    mRightStick.set(0.0f, 0.0f);
    mLeftAnalogTrigger = 0.0f;
    mRightAnalogTrigger = 0.0f;
}

u32 ControllerBase::createStickCrossMask_()
{
    BitFlag32 mask;

    if (mLeftStickCrossStartBit >= 0)
    {
        mask.setBit(mLeftStickCrossStartBit + cCrossUp);
        mask.setBit(mLeftStickCrossStartBit + cCrossDown);
        mask.setBit(mLeftStickCrossStartBit + cCrossLeft);
        mask.setBit(mLeftStickCrossStartBit + cCrossRight);
    }

    if (mRightStickCrossStartBit >= 0)
    {
        mask.setBit(mRightStickCrossStartBit + cCrossUp);
        mask.setBit(mRightStickCrossStartBit + cCrossDown);
        mask.setBit(mRightStickCrossStartBit + cCrossLeft);
        mask.setBit(mRightStickCrossStartBit + cCrossRight);
    }

    return mask.getDirect();
}

}  // namespace sead
