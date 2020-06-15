#ifndef SEAD_NAMABLE_H_
#define SEAD_NAMABLE_H_

#include <prim/seadSafeString.hpp>

namespace sead
{
class INamable
{
public:
    INamable() : mINamableName() {}
    INamable(const SafeString& name) : mINamableName(name) {}

    const SafeString& getName() const { return mINamableName; }
    void setName(const SafeString& name) { mINamableName = name; }

private:
    SafeString mINamableName;
};

}  // namespace sead

#endif  // SEAD_NAMABLE_H_
