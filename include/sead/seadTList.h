#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <sead/seadListImpl.h>

namespace sead {

template <typename T>
class TList
{
public:
    TList()
        : mList()
    {
    }

    void setParentFor(T* item)
    {
        item->parent = &mList;
    }

    void insertFront(T* item)
    {
        mList.mStartEnd.insertFront_(&item->root);
    }

    s32& getCountRef()
    {
        return mList.mCount;
    }

    UnkList* root() const
    {
        return reinterpret_cast<UnkList*>(mList.mStartEnd.mNext);
    }

    static UnkList* next(UnkList* list)
    {
        return reinterpret_cast<UnkList*>(list->root.mNext);
    }

    bool isAtEnd(UnkList* list) const
    {
        return &list->root == &mList.mStartEnd;
    }

    static T* getParent(UnkList* list)
    {
        return static_cast<FileDevice*>(list->ptrToSelf);
    }

private:
    ListImpl mList;
};

} // namespace sead

#endif // SEAD_TLIST_H_
