#include <resource/seadArchiveRes.h>

namespace sead
{
void ArchiveRes::doPostCreate_() {}

s32 ArchiveRes::getLoadDataAlignment() const
{
    return 0x80;
}

void ArchiveRes::doCreate_(u8* buf, u32, Heap*)
{
    mEnable = prepareArchive_(buf);
}

}  // namespace sead
