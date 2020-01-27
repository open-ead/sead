#ifndef SEAD_DISPOSER_H_
#define SEAD_DISPOSER_H_

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

#endif // SEAD_DISPOSER_H_
