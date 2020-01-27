#ifndef SEAD_RUNTIMETYPEINFO_H_
#define SEAD_RUNTIMETYPEINFO_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>

namespace sead {

class RuntimeTypeInfo
{
public:
    class Interface
    {
    public:
        void* isDeriveFunc;
    };
};

} // namespace sead

// TODO: Finish this and add sead::RuntimeTypeInfo::Derive
#define SEAD_RTTI_BASE(CLASS)                                                                       \
    public:                                                                                         \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                   \
        {                                                                                           \
            static const sead::RuntimeTypeInfo::Interface typeInfo;                                 \
            return &typeInfo;                                                                       \
        }                                                                                           \
                                                                                                    \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface*) const     \
        {                                                                                           \
            return true;                                                                            \
        }                                                                                           \
                                                                                                    \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                  \
        {                                                                                           \
            return getRuntimeTypeInfoStatic();                                                      \
        }

#define SEAD_RTTI_OVERRIDE(CLASS, BASE)                                                             \
    public:                                                                                         \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                   \
        {                                                                                           \
            static const sead::RuntimeTypeInfo::Interface typeInfo;                                 \
            return &typeInfo;                                                                       \
        }                                                                                           \
                                                                                                    \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface*) const     \
        {                                                                                           \
            return true;                                                                            \
        }                                                                                           \
                                                                                                    \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                  \
        {                                                                                           \
            return getRuntimeTypeInfoStatic();                                                      \
        }

#endif // SEAD_RUNTIMETYPEINFO_H_
