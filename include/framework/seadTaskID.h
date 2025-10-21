#ifndef SEAD_TASK_ID_H_
#define SEAD_TASK_ID_H_

#include <basis/seadTypes.h>
#include <framework/seadHeapPolicies.h>

namespace sead
{
class HeapArray;
class TaskBase;
class TaskMgr;
class TaskParameter;

struct TaskConstructArg
{
    TaskConstructArg() = default;

    HeapArray* heap_array;
    TaskMgr* mgr;
    TaskParameter* param;
};

typedef TaskBase* (*TaskFactory)(const TaskConstructArg&);

template <typename T = TaskBase>
TaskBase* TTaskFactory(const TaskConstructArg& arg)
{
    return new (arg.heap_array->getPrimaryHeap()) T(arg);
}

class TaskClassID
{
public:
    enum class Type : u32
    {
        cInvalid = 0,
        cInt = 1,
        cFactory = 2,
        cString = 3
    };

    TaskClassID() = default;
    TaskClassID(s32 i) : mType(Type::cInt) { mID.mInt = i; }
    TaskClassID(TaskFactory f) : mType(Type::cFactory) { mID.mFactory = f; }
    TaskClassID(const char* s) : mType(Type::cString) { mID.mString = s; }

public:
    Type mType = Type::cInvalid;
    union
    {
        s32 mInt;
        TaskFactory mFactory;
        const char* mString = nullptr;
    } mID;
};

class TaskUserID
{
public:
    s32 mID = -1;
};

}  // namespace sead

#endif  // SEAD_TASK_ID_H_
