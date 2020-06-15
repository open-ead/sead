#include <basis/seadRawPrint.h>
#include <prim/seadMemUtil.h>
#include <prim/seadPtrUtil.h>

namespace sead
{
void* MemUtil::copyAlign32(void* dst, const void* src, size_t size)
{
    SEAD_ASSERT(size % 32 == 0, "size % 32 == 0  size:%llu", size);
    SEAD_ASSERT(PtrUtil::isAligned(dst, 32) && PtrUtil::isAligned(src, 32),
                "pointer must be 32-byte aligned. src:%#0p -> dst:%#0p", src, dst);
    return copy(dst, src, size);
}

// TODO: MemUtil::isHeap

// TODO: MemUtil::isStack (in platform specific .cpp)

// TODO: MemUtil::dumpMemoryBinary

}  // namespace sead
