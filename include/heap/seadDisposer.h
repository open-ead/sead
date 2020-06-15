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

    static u32 getListNodeOffset() { return offsetof(IDisposer, mListNode); }

protected:
    Heap* getDisposerHeap_() const { return mDisposerHeap; }

private:
    friend class Heap;

    Heap* mDisposerHeap;
    ListNode mListNode;
};

}  // namespace sead

#define SEAD_INSTANCE(CLASS) (CLASS::instance())

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
                                                                                                   \
    static CLASS* instance() { return sInstance; }                                                 \
    static CLASS* createInstance(sead::Heap* heap);                                                \
    static void deleteInstance();                                                                  \
                                                                                                   \
protected:                                                                                         \
    static CLASS* sInstance;                                                                       \
                                                                                                   \
    friend class SingletonDisposer_;                                                               \
                                                                                                   \
    alignas(alignof(SingletonDisposer_))                                                           \
        u32 mSingletonDisposerBuf_[sizeof(SingletonDisposer_) / sizeof(u32)];

#define SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                                      \
    CLASS* CLASS::createInstance(sead::Heap* heap)                                                 \
    {                                                                                              \
        SEAD_ASSERT(!sInstance, "Create Singleton Twice (%s) : addr 0x%016llX", #CLASS,            \
                    reinterpret_cast<uintptr_t>(sInstance));                                       \
        if (!sInstance)                                                                            \
        {                                                                                          \
            auto* instance = reinterpret_cast<CLASS*>(new (heap) u8[sizeof(CLASS)]);               \
            SEAD_ASSERT(!SingletonDisposer_::sStaticDisposer, "Create Singleton Twice (%s).",      \
                        #CLASS);                                                                   \
            auto* staticDisposer =                                                                 \
                reinterpret_cast<SingletonDisposer_*>(instance->mSingletonDisposerBuf_);           \
                                                                                                   \
            SingletonDisposer_::sStaticDisposer = new (staticDisposer) SingletonDisposer_(heap);   \
            sInstance = new (instance) CLASS();                                                    \
        }                                                                                          \
                                                                                                   \
        return CLASS::sInstance;                                                                   \
    }

#define SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                                      \
    void CLASS::deleteInstance()                                                                   \
    {                                                                                              \
        SingletonDisposer_* staticDisposer = SingletonDisposer_::sStaticDisposer;                  \
        if (SingletonDisposer_::sStaticDisposer != NULL)                                           \
        {                                                                                          \
            SingletonDisposer_::sStaticDisposer = NULL;                                            \
            staticDisposer->~SingletonDisposer_();                                                 \
                                                                                                   \
            if (sInstance != NULL)                                                                 \
                delete sInstance;                                                                  \
                                                                                                   \
            sInstance = NULL;                                                                      \
        }                                                                                          \
    }

#define SEAD_SINGLETON_DISPOSER_IMPL(CLASS)                                                        \
    SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                                          \
    SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                                          \
    CLASS* CLASS::sInstance = NULL;                                                                \
    CLASS::SingletonDisposer_* CLASS::SingletonDisposer_::sStaticDisposer = NULL;

#endif  // SEAD_DISPOSER_H_
