#ifndef SEAD_DISPOSER_H_
#define SEAD_DISPOSER_H_

#include <sead/seadListImpl.h>

namespace sead {

class Heap;

class IDisposer
{
public:
    IDisposer();
    virtual ~IDisposer();

private:
    friend class Heap;

    Heap* containHeap;
    ListNode listNode;
};

} // namespace sead

#define SEAD_SINGLETON_DISPOSER(CLASS, INSTANCE)            \
    public:                                                 \
        class SingletonDisposer_ : public IDisposer         \
        {                                                   \
        public:                                             \
            SingletonDisposer_() : IDisposer() { }          \
            virtual ~SingletonDisposer_()                   \
            {                                               \
                if (this == sStaticDisposer)                \
                {                                           \
                    sStaticDisposer = NULL;                 \
                    INSTANCE->~CLASS();                     \
                    INSTANCE = NULL;                        \
                }                                           \
            }                                               \
                                                            \
            static SingletonDisposer_* sStaticDisposer;     \
        };                                                  \
                                                            \
        static CLASS* createInstance(Heap* heap);           \
        static void deleteInstance();                       \
                                                            \
        u8 disposer[sizeof(SingletonDisposer_)];

#define SEAD_CREATE_SINGLETON_INSTANCE(CLASS, INSTANCE)                                                 \
    CLASS* CLASS::createInstance(Heap* heap)                                                            \
    {                                                                                                   \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;         \
        CLASS* instance = INSTANCE;                                                                     \
                                                                                                        \
        if (INSTANCE == NULL)                                                                           \
        {                                                                                               \
            instance = reinterpret_cast<CLASS*>(new(heap, 4) u8[sizeof(CLASS)]);                        \
            staticDisposer = reinterpret_cast<CLASS::SingletonDisposer_*>(instance->disposer);          \
                                                                                                        \
            CLASS::SingletonDisposer_::sStaticDisposer = new (staticDisposer) SingletonDisposer_();     \
            INSTANCE = new (instance) CLASS();                                                          \
        }                                                                                               \
                                                                                                        \
        return INSTANCE;                                                                                \
    }

#define SEAD_DELETE_SINGLETON_INSTANCE(CLASS, INSTANCE)                                             \
    void CLASS::deleteInstance()                                                                    \
    {                                                                                               \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;     \
        if (CLASS::SingletonDisposer_::sStaticDisposer != NULL)                                     \
        {                                                                                           \
            CLASS::SingletonDisposer_::sStaticDisposer = NULL;                                      \
            staticDisposer->~SingletonDisposer_();                                                  \
                                                                                                    \
            if (INSTANCE != NULL)                                                                   \
                delete INSTANCE;                                                                    \
                                                                                                    \
            INSTANCE = NULL;                                                                        \
        }                                                                                           \
    }

#endif // SEAD_DISPOSER_H_
