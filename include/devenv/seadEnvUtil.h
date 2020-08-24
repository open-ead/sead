#pragma once

#include "prim/seadSafeString.h"

namespace sead
{
class EnvUtil
{
public:
    static s32 getEnvironmentVariable(BufferedSafeString* out, const SafeString& variable);
};
}  // namespace sead
