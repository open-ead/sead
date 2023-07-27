#pragma once

#include <framework/seadMethodTree.h>
#include <framework/seadTaskBase.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead
{
class Task : public TaskBase
{
    SEAD_RTTI_BASE(TaskBase);

public:
    explicit Task(const TaskConstructArg& arg);
    Task(const TaskConstructArg& arg, const char* name);
    ~Task() override;
    void pauseCalc(bool b) override;
    void pauseDraw(bool b) override;
    void pauseCalcRec(bool b) override;
    void pauseDrawRec(bool b) override;
    void pauseCalcChild(bool b) override;
    void pauseDrawChild(bool b) override;
    void attachCalcImpl() override;
    void attachDrawImpl() override;
    void detachCalcImpl() override;
    void detachDrawImpl() override;
    const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const override;
    MethodTreeNode* getMethodTreeNode(s32 method_type) override;
    virtual void calc() {}
    virtual void draw() {}

protected:
    MethodTreeNode mCalcNode{nullptr};
    MethodTreeNode mDrawNode{nullptr};
};
}  // namespace sead
