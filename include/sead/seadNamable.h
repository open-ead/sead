#ifndef SEAD_NAMABLE_H_
#define SEAD_NAMABLE_H_

#include <sead/seadSafeString.hpp>

namespace sead
{
class INamable
{
public:
    INamable() : mINamableName() {}

    INamable(const char* str) : mINamableName(str) {}

    ~INamable() {}

    SafeString mINamableName;
};

}  // namespace sead

#endif  // SEAD_NAMABLE_H_
