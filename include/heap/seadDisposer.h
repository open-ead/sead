#ifndef SEAD_DISPOSER_H_
#define SEAD_DISPOSER_H_

#include <basis/seadNew.h>
#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>
#include <container/seadListImpl.h>

namespace sead
{
class Heap;

class IDisposer
{
public:
    enum class HeapNullOption
    {
        // disposer_heap must not be nullptr for this option.
        AlwaysUseSpecifiedHeap = 0,
        UseSpecifiedOrContainHeap = 1,
        DoNotAppendDisposerIfNoHeapSpecified = 2,
        UseSpecifiedOrCurrentHeap = 3,
    };

    IDisposer();
    IDisposer(Heap* disposer_heap,
              HeapNullOption option = HeapNullOption::UseSpecifiedOrCurrentHeap);
    virtual ~IDisposer();

private:
    friend class Heap;

    Heap* mDisposerHeap;
    ListNode mListNode;
};

}  // namespace sead

#define SEAD_SINGLETON_DISPOSER(CLASS)                                                             \
public:                                                                                            \
    class SingletonDisposer_ : public sead::IDisposer                                              \
    {                                                                                              \
    public:                                                                                        \
        using sead::IDisposer::IDisposer;                                                          \
        virtual ~SingletonDisposer_()                                                              \
        {                                                                                          \
            if (this == sStaticDisposer)                                                           \
            {                                                                                      \
                sStaticDisposer = nullptr;                                                         \
                CLASS::sInstance->~CLASS();                                                        \
                CLASS::sInstance = nullptr;                                                        \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        static SingletonDisposer_* sStaticDisposer;                                                \
    };                                                                                             \
    static CLASS* sInstance;                                                                       \
                                                                                                   \
    static CLASS* createInstance(sead::Heap* heap);                                                \
    static void deleteInstance();                                                                  \
                                                                                                   \
    alignas(alignof(SingletonDisposer_))                                                           \
        u32 mSingletonDisposerBuf_[sizeof(SingletonDisposer_) / sizeof(u32)];

#define SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                                      \
    CLASS* CLASS::createInstance(sead::Heap* heap)                                                 \
    {                                                                                              \
        SEAD_ASSERT(!CLASS::sInstance, "Create Singleton Twice (%s) : addr 0x%016llX", #CLASS,     \
                    reinterpret_cast<uintptr_t>(CLASS::sInstance));                                \
        if (!CLASS::sInstance)                                                                     \
        {                                                                                          \
            auto* instance = reinterpret_cast<CLASS*>(new (heap) u8[sizeof(CLASS)]);               \
            SEAD_ASSERT(!CLASS::SingletonDisposer_::sStaticDisposer,                               \
                        "Create Singleton Twice (%s).", #CLASS);                                   \
            auto* staticDisposer =                                                                 \
                reinterpret_cast<CLASS::SingletonDisposer_*>(instance->mSingletonDisposerBuf_);    \
                                                                                                   \
            CLASS::SingletonDisposer_::sStaticDisposer =                                           \
                new (staticDisposer) SingletonDisposer_(heap);                                     \
            CLASS::sInstance = new (instance) CLASS();                                             \
        }                                                                                          \
                                                                                                   \
        return CLASS::sInstance;                                                                   \
    }

#define SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                                      \
    void CLASS::deleteInstance()                                                                   \
    {                                                                                              \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;    \
        if (CLASS::SingletonDisposer_::sStaticDisposer != NULL)                                    \
        {                                                                                          \
            CLASS::SingletonDisposer_::sStaticDisposer = NULL;                                     \
            staticDisposer->~SingletonDisposer_();                                                 \
                                                                                                   \
            if (CLASS::sInstance != NULL)                                                          \
                delete CLASS::sInstance;                                                           \
                                                                                                   \
            CLASS::sInstance = NULL;                                                               \
        }                                                                                          \
    }

#endif  // SEAD_DISPOSER_H_
