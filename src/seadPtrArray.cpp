#include <sead.h>

namespace sead {

void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax < 1)
        return;

    if (buf == NULL)
        return;

    mPtrNumMax = ptrNumMax;
    mPtrs = static_cast<void**>(buf);
    mPtrNum = 0;
}

} // namespace sead
