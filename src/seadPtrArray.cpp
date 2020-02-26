#include <sead.h>

namespace sead {

void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax < 1)
        return;

    if (buf == NULL)
        return;

    mPtrNumMax = ptrNumMax;
    mPtrs = buf;
    mSize = NULL;
}

} // namespace sead
