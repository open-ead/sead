#include "controller/seadPatternRumbleAddon.h"

#include "basis/seadRawPrint.h"

// unchecked copy from Wii U-decomp
namespace sead
{
PatternRumbleAddon::PatternRumbleAddon(Controller* controller)
    : ControllerAddon(controller), mPattern(nullptr), mPatternIdx(0), mPatternDuration(0)
{
    mId = ControllerDefine::cAddon_PatternRumble;
}

bool PatternRumbleAddon::isPatternEnable() const
{
    return mPattern != nullptr;
}

void PatternRumbleAddon::startPattern(const char* pattern, u32 duration)
{
    SEAD_ASSERT(pattern != nullptr);

    mPattern = pattern;
    mPatternIdx = 0;
    mPatternDuration = duration;
}

void PatternRumbleAddon::stopPattern()
{
    mPattern = nullptr;
    mPatternIdx = 1;
    mPatternDuration = 0;
}

bool PatternRumbleAddon::calc()
{
    if (mPattern)
    {
        if (mPattern[mPatternIdx] == '\0')
        {
            mPatternIdx = 0;

            if (mPatternDuration)
            {
                if (mPatternDuration == 1)
                {
                    mPattern = nullptr;
                    mPatternDuration = 0;
                    stopRumbleImpl_();
                    return false;
                }

                mPatternDuration--;
            }
        }

        if (mPattern[mPatternIdx] == '*')
            startRumbleImpl_();

        else
            stopRumbleImpl_();

        mPatternIdx++;
    }
    else
    {
        if (mPatternIdx)
        {
            stopRumbleImpl_();
            mPatternIdx = 0;
        }
    }

    return false;
}

}  // namespace sead
