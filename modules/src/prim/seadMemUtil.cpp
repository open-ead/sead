#include <basis/seadRawPrint.h>
#include <prim/seadMemUtil.h>
#include <prim/seadPtrUtil.h>

namespace sead
{
void* MemUtil::copyAlign32(void* dst, const void* src, size_t size)
{
    SEAD_ASSERT_MSG(size % 32 == 0, "size %% 32 == 0  size:%zu", size);
    SEAD_ASSERT_MSG(PtrUtil::isAligned(dst, 32) && PtrUtil::isAligned(src, 32),
                    "pointer must be 32-byte aligned. src:%p -> dst:%p", src, dst);
    return copy(dst, src, size);
}

// TODO: MemUtil::isHeap

// TODO: MemUtil::isStack (in platform specific .cpp)

// TODO: MemUtil::dumpMemoryBinary

}  // namespace sead
