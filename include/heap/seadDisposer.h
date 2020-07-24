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
        if (!sInstance)                                                                            \
        {                                                                                          \
            auto* buffer = new (heap) u8[sizeof(CLASS)];                                           \
            SEAD_ASSERT_MSG(!SingletonDisposer_::sStaticDisposer, "Create Singleton Twice (%s).",  \
                            #CLASS);                                                               \
            auto* disposer_buffer = buffer + offsetof(CLASS, mSingletonDisposerBuf_);              \
                                                                                                   \
            /* FIXME: this is UB and actually dangerous */                                         \
            SingletonDisposer_::sStaticDisposer = new (disposer_buffer) SingletonDisposer_(heap);  \
            /* Note: This must not be new (buffer) CLASS() or {} as that will zero initialize      \
             * every member when CLASS has no user-provided constructor. This is especially        \
             * dangerous because the singleton disposer will get clobbered */                      \
            sInstance = new (buffer) CLASS;                                                        \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            SEAD_ASSERT_MSG(false, "Create Singleton Twice (%s) : addr %p", #CLASS, sInstance);    \
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
