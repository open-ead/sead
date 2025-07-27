#include "framework/seadTaskBase.h"

namespace sead
{
TaskBase::CreateArg::CreateArg() = default;

TaskBase::CreateArg::CreateArg(const TaskClassID& factory) : factory(factory) {}
}  // namespace sead
