#ifndef SEAD_RUNTIMETYPEINFO_H_
#define SEAD_RUNTIMETYPEINFO_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>

namespace sead { namespace RuntimeTypeInfo {

class Interface
{
public:
    Interface() { }
};

class Root : public Interface
{
public:
    Root() { }

    virtual bool isDerived(const Interface* typeInfo) const
    {
        return typeInfo == static_cast<const Interface*>(this);
    }
};

template <typename BaseType>
class Derive : public Root
{
public:
    Derive() { }

    virtual bool isDerived(const Interface* typeInfo) const
    {
        if (static_cast<const Interface*>(this) == typeInfo)
            return true;

        const RuntimeTypeInfo::Root* rootTypeInfo = reinterpret_cast<const Root*>(BaseType::getRuntimeTypeInfoStatic());
        return rootTypeInfo->isDerived(typeInfo);
    }
};

} // namespace sead::RuntimeTypeInfo

template <typename DerivedType, typename Type>
inline bool
IsDerivedFrom(const Type* obj)
{
    const RuntimeTypeInfo::Interface* typeInfo = DerivedType::getRuntimeTypeInfoStatic();
    return obj != NULL && obj->checkDerivedRuntimeTypeInfo(typeInfo);
}

} // namespace sead

#define SEAD_RTTI_BASE(CLASS)                                                                                \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                            \
        {                                                                                                    \
            static const sead::RuntimeTypeInfo::Root typeInfo;                                               \
            return &typeInfo;                                                                                \
        }                                                                                                    \
                                                                                                             \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* typeInfo) const     \
        {                                                                                                    \
            const RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();               \
            return typeInfo == clsTypeInfo;                                                                  \
        }                                                                                                    \
                                                                                                             \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                           \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#define SEAD_RTTI_OVERRIDE(CLASS, BASE)                                                                      \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                            \
        {                                                                                                    \
            static const sead::RuntimeTypeInfo::Derive<BASE> typeInfo;                                       \
            return &typeInfo;                                                                                \
        }                                                                                                    \
                                                                                                             \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* typeInfo) const     \
        {                                                                                                    \
            const RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();               \
            if (typeInfo == clsTypeInfo)                                                                     \
                return true;                                                                                 \
                                                                                                             \
            return BASE::checkDerivedRuntimeTypeInfo(typeInfo);                                              \
        }                                                                                                    \
                                                                                                             \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                           \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#endif // SEAD_RUNTIMETYPEINFO_H_
