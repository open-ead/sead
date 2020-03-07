#include <sead.h>

namespace sead {

FileDevice::~FileDevice()
{
    if (FileDeviceMgr::sInstance != NULL)
        FileDeviceMgr::sInstance->unmount(this);
}

} // namespace sead
