#pragma once

#include <type_traits>

#include "prim/seadBitFlag.h"

namespace sead
{
template <typename Enum>
class TypedBitFlag
{
public:
    static_assert(std::is_enum<Enum>(), "Enum must be an enum");
    using UnderlyingType = std::underlying_type_t<Enum>;

    TypedBitFlag() : mBits(0) {}
    TypedBitFlag(UnderlyingType bits) : mBits(bits) {}

    void makeAllZero() { mBits = 0; }
    void makeAllOne() { mBits = std::numeric_limits<UnderlyingType>::max(); }

    void setDirect(UnderlyingType bits) { mBits = bits; }
    UnderlyingType getDirect() const { return mBits; }

    void set(Enum val) { mBits |= UnderlyingType(val); }
    void reset(Enum val) { mBits &= ~UnderlyingType(val); }
    void toggle(Enum val) { mBits ^= UnderlyingType(val); }
    void change(Enum val, bool on)
    {
        if (on)
            set(val);
        else
            reset(val);
    }
    bool isZero() const { return mBits == 0; }
    /// Checks if (at least) one of the bits are set.
    bool isOn(Enum val) const { return (mBits & UnderlyingType(val)) != 0; }
    /// Checks if all of the bits are set.
    bool isOnAll(Enum val) const { return (mBits & UnderlyingType(val)) == UnderlyingType(val); }
    bool isOff(Enum val) const { return !isOn(val); }

    bool testAndClear(Enum val)
    {
        if (!isOn(val))
            return false;
        reset(val);
        return true;
    }

    /// Popcount.
    int countOnBit() const
    {
        if constexpr (sizeof(UnderlyingType) <= 4)
            return BitFlagUtil::countOnBit(mBits);
        else
            return BitFlagUtil::countOnBit64(mBits);
    }
    /// Count trailing zeroes.
    int countContinuousOffBitFromRight() const
    {
        if constexpr (sizeof(UnderlyingType) <= 4)
            return BitFlagUtil::countContinuousOffBitFromRight(mBits);
        else
            return BitFlagUtil::countContinuousOffBitFromRight64(mBits);
    }
    int countRightOnBit(int bit) const
    {
        if constexpr (sizeof(UnderlyingType) <= 4)
            return BitFlagUtil::countRightOnBit(mBits, bit);
        else
            return BitFlagUtil::countRightOnBit64(mBits, bit);
    }
    int findOnBitFromRight(int num) const
    {
        if constexpr (sizeof(UnderlyingType) <= 4)
            return BitFlagUtil::findOnBitFromRight(mBits, num);
        else
            return BitFlagUtil::findOnBitFromRight64(mBits, num);
    }

protected:
    UnderlyingType mBits;
};
}  // namespace sead
