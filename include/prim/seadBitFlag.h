#ifndef SEAD_BIT_FLAG_H_
#define SEAD_BIT_FLAG_H_

#include <basis/seadTypes.h>

namespace sead
{
template <typename T>
class BitFlag
{
public:
    BitFlag() : mBits(0) {}

    T mBits;
};

typedef BitFlag<u32> BitFlag32;

}  // namespace sead

#endif  // SEAD_BIT_FLAG_H_
